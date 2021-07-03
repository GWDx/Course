#include <linux/err.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/wait.h>

#include <linux/freezer.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pid.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/types.h>

#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <linux/rmap.h>
#include <linux/sched.h>
#include <linux/signal.h>

#include <asm/pgtable.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GWDx");
MODULE_DESCRIPTION("kmscan");

//sysfs
#define MEM_RUN_STOP 0
#define MEM_RUN_START 1

static unsigned int kmscan_pid = 0;                       //  /sys/kerbel/mm/kmscan/pid
static unsigned int kmscan_func = 0;                      //  /sys/kerbel/mm/kmscan/func
static unsigned int kmscan_run = MEM_RUN_STOP;            //  /sys/kernel/mm/kmscan/kmscan_run
static unsigned int kmscan_thread_sleep_millisecs = 5000; //  /sys/kernel/mm/kmscan/sleep_millisecs

static struct task_struct *kmscan_thread;
static DECLARE_WAIT_QUEUE_HEAD(kmscan_thread_wait);
static DEFINE_MUTEX(kmscan_thread_mutex);

// proc
static struct proc_dir_entry *_kmscan, *_kmscan_pid;

#define mySize 6000
typedef struct {
    long data[mySize + 3];
    int length;
} myArray;

static int testShowInt(struct seq_file *m, void *v) {
    int *value = (int *)m->private;
    if (value != NULL)
        seq_printf(m, "%d\n", *value);
    return 0;
}
static int testShowArray(struct seq_file *m, void *v) {
    long *data;
    int length, i;
    myArray *pfn = (myArray *)m->private;
    if (pfn != NULL) {
        length = min(pfn->length, mySize);
        data = pfn->data;
        for (i = 0; i < length; i++)
            seq_printf(m, "%lx\t", data[i]);
        seq_printf(m, "\n");
    }
    return 0;
}

static int testOpenInt(struct inode *inode, struct file *file) {
    return single_open(file, testShowInt, PDE_DATA(inode));
}
static int testOpenArray(struct inode *inode, struct file *file) {
    return single_open(file, testShowArray, PDE_DATA(inode));
}

// linux5.9.0
// 低版本需要查看proc_fs.h文件定义的结构体
struct proc_ops testOpsInt = {
    .proc_open = testOpenInt,
    .proc_read = seq_read,
    .proc_release = single_release,
};
struct proc_ops testOpsArray = {
    .proc_open = testOpenArray,
    .proc_read = seq_read,
    .proc_release = single_release,
};

static int kmscand_should_run(void) {
    return (kmscan_run & MEM_RUN_START);
}

int makeDir(unsigned int pid) {
    char pidStr[10];
    _kmscan = proc_mkdir("kmscan", NULL);
    if (_kmscan == NULL) {
        printk(KERN_ALERT "%s proc create %s failed\n", __func__, "test");
        return -EINVAL;
    }

    sprintf(pidStr, "%u", pid);
    _kmscan_pid = proc_mkdir(pidStr, _kmscan);
    if (_kmscan_pid == NULL) {
        printk(KERN_ALERT "%s proc create %s failed\n", __func__, "test");
        return -EINVAL;
    }
    return 0;
}

void removeDir(void) {
    proc_remove(_kmscan_pid);
    proc_remove(_kmscan);
}

void tallyVMA(unsigned int pid) {
    struct pid *PID;
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *firstVMA, *VMA;
    static int vmaCount; // ?

    PID = find_get_pid(pid);
    if (!PID)
        return;
    task = pid_task(PID, PIDTYPE_PID);
    vmaCount = 0;
    if (task) {
        mm = task->mm;
        firstVMA = mm->mmap;
        for (VMA = firstVMA; VMA; VMA = VMA->vm_next)
            vmaCount++;
    }
    printk(KERN_ALERT "vmaCount= %d\n", vmaCount);

    removeDir();
    makeDir(pid);

    proc_create_data("vma_count", 0664, _kmscan_pid, &testOpsInt, &vmaCount);
}

typedef typeof(follow_page) *my_follow_page;
typedef typeof(page_referenced) *my_page_referenced;

void tallyPage(unsigned int pid) {
    struct pid *PID;
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *firstVMA, *VMA;
    unsigned long address;
    struct page *targePage;
    int active, anon;
    static int fileCount, activeFileCoint, anonCount, activeAnonCount;

    my_follow_page mfollow_page;
    my_page_referenced mpage_referenced;
    mfollow_page = (my_follow_page)0xffffffffb587e1e0;
    mpage_referenced = (my_page_referenced)0xffffffffb5899330;

    PID = find_get_pid(pid);
    if (!PID)
        return;
    task = pid_task(PID, PIDTYPE_PID);
    fileCount = activeFileCoint = anonCount = activeAnonCount = 0;
    if (!task)
        return;
    mm = task->mm;
    firstVMA = mm->mmap;
    for (VMA = firstVMA; VMA; VMA = VMA->vm_next) {
        // printk(KERN_ALERT "vmaNumber= %lu %lu\n", VMA->vm_start, VMA->vm_end);
        for (address = VMA->vm_start; address < VMA->vm_end; address += PAGE_SIZE) {
            targePage = mfollow_page(VMA, address, FOLL_GET);
            if (!targePage) {
                // printk(KERN_ALERT "Page address:%lu\tNULL\n", address);
                continue;
            }
            active = mpage_referenced(targePage, 1, targePage->mem_cgroup, &VMA->vm_flags);
            anon = PageAnon(targePage);
            if (!anon)
                fileCount++;
            if (!anon && active)
                activeFileCoint++;
            if (anon)
                anonCount++;
            if (anon && active)
                activeAnonCount++;
        }
    }
    printk(KERN_ALERT "fileCount= %d\n", fileCount);
    printk(KERN_ALERT "activeFileCoint= %d\n", activeFileCoint);
    printk(KERN_ALERT "anonCount= %d\n", anonCount);
    printk(KERN_ALERT "activeAnonCount= %d\n", activeAnonCount);
    printk(KERN_ALERT "\n");

    removeDir();
    makeDir(pid);

    proc_create_data("file", 0664, _kmscan_pid, &testOpsInt, &fileCount);
    proc_create_data("active_file", 0664, _kmscan_pid, &testOpsInt, &activeFileCoint);
    proc_create_data("anon", 0664, _kmscan_pid, &testOpsInt, &anonCount);
    proc_create_data("active_anon", 0664, _kmscan_pid, &testOpsInt, &activeAnonCount);
}

void visitPage(unsigned int pid) {
    struct pid *PID;
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *firstVMA, *VMA;
    unsigned long address, pfn;
    static myArray pfnWrite;

    pgd_t *pgdOffset;
    p4d_t *p4dOffset;
    pud_t *pudOffset;
    pmd_t *pmdOffset;
    pte_t *pteOffset;

    PID = find_get_pid(pid);
    if (!PID)
        return;
    task = pid_task(PID, PIDTYPE_PID);
    pfnWrite.length = 0;
    if (!task)
        return;
    mm = task->mm;
    firstVMA = mm->mmap;
    for (VMA = firstVMA; VMA; VMA = VMA->vm_next)
        for (address = VMA->vm_start; address < VMA->vm_end; address += PAGE_SIZE) {
            pgdOffset = pgd_offset(mm, address);
            p4dOffset = p4d_offset(pgdOffset, address);
            pudOffset = pud_offset(p4dOffset, address);
            pmdOffset = pmd_offset(pudOffset, address);
            pteOffset = pte_offset_kernel(pmdOffset, address);
            pfn = (pteOffset->pte & 4294967295) >> 12;
            if (pfn) {
                if (pfnWrite.length < mySize)
                    pfnWrite.data[pfnWrite.length] = pfn;
                pfnWrite.length++;
            }

            printk(KERN_ALERT "%lx\t%lx\t%lu\t%lu\t%lu\t%lu\n", address, pfn,
                   pgdOffset->pgd, pudOffset->pud, pmdOffset->pmd, pteOffset->pte);
        }

    printk(KERN_ALERT "%d\n", pfnWrite.length);

    removeDir();
    makeDir(pid);

    proc_create_data("pfn", 0664, _kmscan_pid, &testOpsArray, &pfnWrite);
}

static void kmscan_to_do(void) {
    if (kmscan_func == 1)
        tallyVMA(kmscan_pid);
    else if (kmscan_func == 2)
        tallyPage(kmscan_pid);
    else if (kmscan_func == 3)
        visitPage(kmscan_pid);
}

static int kmscand_thread(void *nothing) {
    set_freezable();
    set_user_nice(current, 5);
    while (!kthread_should_stop()) {
        mutex_lock(&kmscan_thread_mutex);
        if (kmscand_should_run())
            kmscan_to_do();
        mutex_unlock(&kmscan_thread_mutex);
        try_to_freeze();
        if (kmscand_should_run()) {
            schedule_timeout_interruptible(
                msecs_to_jiffies(kmscan_thread_sleep_millisecs));
        } else {
            wait_event_freezable(kmscan_thread_wait,
                                 kmscand_should_run() || kthread_should_stop());
        }
    }
    return 0;
}

#ifdef CONFIG_SYSFS

/*
 * This all compiles without CONFIG_SYSFS, but is a waste of space.
 */

#define MEM_ATTR_RO(_name) \
    static struct kobj_attribute _name##_attr = __ATTR_RO(_name)

#define MEM_ATTR(_name)                         \
    static struct kobj_attribute _name##_attr = \
        __ATTR(_name, 0644, _name##_show, _name##_store)

static ssize_t sleep_millisecs_show(struct kobject *kobj,
                                    struct kobj_attribute *attr,
                                    char *buf) {
    return sprintf(buf, "%u\n", kmscan_thread_sleep_millisecs);
}

static ssize_t sleep_millisecs_store(struct kobject *kobj,
                                     struct kobj_attribute *attr,
                                     const char *buf,
                                     size_t count) {
    unsigned long msecs;
    int err;

    err = kstrtoul(buf, 10, &msecs);
    if (err || msecs > UINT_MAX)
        return -EINVAL;

    kmscan_thread_sleep_millisecs = msecs;

    return count;
}
MEM_ATTR(sleep_millisecs);

static ssize_t pid_show(struct kobject *kobj,
                        struct kobj_attribute *attr,
                        char *buf) {
    return sprintf(buf, "%u\n", kmscan_pid);
}

static ssize_t pid_store(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         const char *buf,
                         size_t count) {
    unsigned long tmp;
    int err;

    err = kstrtoul(buf, 10, &tmp);
    if (err || tmp > UINT_MAX)
        return -EINVAL;

    kmscan_pid = tmp;

    return count;
}
MEM_ATTR(pid);

static ssize_t func_show(struct kobject *kobj,
                         struct kobj_attribute *attr,
                         char *buf) {
    return sprintf(buf, "%u\n", kmscan_func);
}

static ssize_t func_store(struct kobject *kobj,
                          struct kobj_attribute *attr,
                          const char *buf,
                          size_t count) {
    unsigned long tmp;
    int err;

    err = kstrtoul(buf, 10, &tmp);
    if (err || tmp > UINT_MAX)
        return -EINVAL;

    kmscan_func = tmp;

    return count;
}
MEM_ATTR(func);

static ssize_t run_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%u\n", kmscan_run);
}

static ssize_t run_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    int err;
    unsigned long flags;
    err = kstrtoul(buf, 10, &flags);
    if (err || flags > UINT_MAX)
        return -EINVAL;
    if (flags > MEM_RUN_START)
        return -EINVAL;
    mutex_lock(&kmscan_thread_mutex);
    if (kmscan_run != flags) {
        kmscan_run = flags;
    }
    mutex_unlock(&kmscan_thread_mutex);

    if (flags & MEM_RUN_START)
        wake_up_interruptible(&kmscan_thread_wait);
    return count;
}
MEM_ATTR(run);

static struct attribute *kmscan_attrs[] = {
    // 扫描进程的扫描间隔 默认为20秒
    &sleep_millisecs_attr.attr,
    &pid_attr.attr,
    &func_attr.attr,
    &run_attr.attr,
    NULL,
};

static struct attribute_group kmscan_attr_group = {
    .attrs = kmscan_attrs,
    .name = "kmscan",
};
#endif /* CONFIG_SYSFS */

static int kmscan_init(void) {
    int err;

    makeDir(kmscan_pid); //

    kmscan_thread = kthread_run(kmscand_thread, NULL, "kmscan");
    if (IS_ERR(kmscan_thread)) {
        pr_err("kmscan: creating kthread failed\n");
        err = PTR_ERR(kmscan_thread);
        goto out;
    }

#ifdef CONFIG_SYSFS
    err = sysfs_create_group(mm_kobj, &kmscan_attr_group);
    if (err) {
        pr_err("kmscan: register sysfs failed\n");
        kthread_stop(kmscan_thread);
        goto out;
    }
#else
    kmscan_run = KSCAN_RUN_STOP;
#endif /* CONFIG_SYSFS */

out:
    return err;
}

static void kmscan_exit(void) {
    removeDir();

    if (kmscan_thread) {
        kthread_stop(kmscan_thread);
        kmscan_thread = NULL;
    }

#ifdef CONFIG_SYSFS

    sysfs_remove_group(mm_kobj, &kmscan_attr_group);

#endif

    printk("kmscan exit success!\n");
}

/* --- 随内核启动  ---  */
// subsys_initcall(kscan_init);
module_init(kmscan_init);
module_exit(kmscan_exit);
