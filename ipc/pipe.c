#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    int fd[2]; // fd[0]读端 fd[1]写端
    int len;
    char str[64] = "hello pipe";
    char buf[64];

    pid_t pid;

    if(pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    pid = fork();

    // 父写子读
    if(pid > 0) {
        // 父进程，关闭父读，关闭子写
        printf("I'm parent\n");
        close(fd[0]);
        write(fd[1], str, strlen(str));
        // 回收子进程资源
        wait(NULL);
    } else if(pid == 0) {
        printf("I'm child:\n");
        close(fd[1]);
        len = read(fd[0], buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);
        putchar('\n');
        // sprintf(str, "child %s", buf);
        // write(STDOUT_FILENO, str, strlen(str));
    }else {
        perror("fork");
        exit(1);
    }
    return 0;
}