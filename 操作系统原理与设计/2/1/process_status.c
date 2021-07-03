#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#define FOR(i, a, b) for (i = a; i <= b; i++)

int main(void) {
    int num,i;
    pid_t pid[105];
    long time[105];

    syscall(333, &num, pid, time);
    printf("process num is %d\n", num);
    printf("PID\tTIME/ms\n");

    FOR(i, 0, num - 1)
        printf("%d\t%ld\n", pid[i], time[i]);
    return 0;
}
