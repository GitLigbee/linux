#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid, pid_c;

    pid = fork();

    if(pid > 0) {
        while(1) {
            printf("I'm parent: %d\n", getpid());
            pid_c = wait(NULL); // wait是一个阻塞函数，如果有子进程，等待回收子进程资源，返回子进程ID，如果没有子进程，wait返回-1
            printf("wait for child:%d\n", pid_c);
            sleep(1);
        }
    }else if(pid == 0) {
        printf("I'm child %d\n", getpid());
        sleep(10);
    }else {
        perror("fork");
    }
    return 0;
}