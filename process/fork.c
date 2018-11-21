#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid;
    // fork 创建子进程后，此时相当于有两个进程在执行，父子进程都会有return值
    pid = fork();

    if(pid > 0) {
        while(1) {
            printf("I am parent \n");
            sleep(1);
        }
    }else if(pid == 0) {
        while(1) {
            printf("I am child \n");
            sleep(5);
        }
    }else {
        perror("fork");
        exit(1);
    }
    return 0;
}