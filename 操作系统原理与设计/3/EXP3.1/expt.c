// 必备头函数
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/timer.h>

// 该模块的LICENSE
MODULE_LICENSE("GPL");
// 该模块的作者
MODULE_AUTHOR("GWDx");
// 该模块的说明
MODULE_DESCRIPTION("expt");

// 该模块需要传递的参数
static int func = 1;
static int pid = 0;
module_param(func, int, 0644);
module_param(pid, int, 0644);

struct timer_list timer;

void timerFunction(struct timer_list *t) {
    struct task_struct *task;
    int count = 0;

    for_each_process(task) {
        if (task->mm == NULL)
            count++;
    }
    printk(KERN_ALERT "%d\n", count);

    t->expires = jiffies + 5 * HZ;
    add_timer(t);
}

// 初始化入口
// 模块安装时执行
// 这里的__init 同样是宏定义，主要的目的在于
// 告诉内核，加载该模块之后，可以回收init.text的区间
static int __init expt_init(void) {
    // 输出信息，类似于printf()
    // printk适用于内核模块
    printk(KERN_ALERT "expt_init()\n");
    if (func == 1) {
        struct task_struct *task;
        printk(KERN_ALERT "PID\tSTATE\tCOMMAND\n");
        for_each_process(task) {
            if (task->mm == NULL)
                printk(KERN_ALERT "%d\t%ld\t%s\n", task->pid, task->state, task->comm);
        }
    } else if (func == 2) {
        timer_setup(&timer, timerFunction, 0);
        timer.expires = jiffies + 5 * HZ;
        add_timer(&timer);
    } else if (func == 3) {
        struct pid *PID;
        struct task_struct *task, *target, *thread;
        struct list_head *targerList;

        printk(KERN_ALERT "pid= %d", pid);
        PID = find_get_pid(pid);
        if (!PID)
            return 1;
        task = pid_task(PID, PIDTYPE_PID);
        if (!task)
            return 1;
        target = task->parent;
        printk(KERN_ALERT "Father:\tpid= %d\tstate= %ld\tcomm= %s\n", target->pid, target->state, target->comm);

        list_for_each(targerList, &(task->children)) {
            target = list_entry(targerList, struct task_struct, sibling);
            printk(KERN_ALERT "Child:\tpid= %d\tstate= %ld\tcomm= %s\n", target->pid, target->state, target->comm);
        }

        list_for_each(targerList, &(task->parent->children)) {
            target = list_entry(targerList, struct task_struct, sibling);
            if (target != task)
                printk(KERN_ALERT "Sibling:\tpid= %d\tstate= %ld\tcomm= %s\n", target->pid, target->state, target->comm);
        }

        thread = task;
        while_each_thread(task, thread) {
            target = thread;
            printk(KERN_ALERT "thread:\tpid= %d\tstate= %ld\tcomm= %s\n", target->pid, target->state, target->comm);
        }
    }
    return 0;
}

// 模块卸载时执行
// 同上
static void __exit expt_exit(void) {
    printk(KERN_ALERT "expt_exit()\n");
    if (func == 2)
        del_timer_sync(&timer);
}

// 模块初始化宏，用于加载该模块
module_init(expt_init);
// 模块卸载宏，用于卸载该模块
module_exit(expt_exit);
