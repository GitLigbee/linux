#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

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
        sleep(5);
        printf("I'm parent\n");
        close(fd[0]);
        write(fd[1], str, strlen(str));
        close(fd[1]);
        // 回收子进程资源
        wait(NULL);
    } else if(pid == 0) {
        printf("I'm child:\n");
        close(fd[1]);
        // 修改为非阻塞
        int flags;
        flags = fcntl(fd[0], F_GETFL);
        flags = flags | O_NONBLOCK;
        fcntl(fd[0], F_SETFL, flags);
        // 判断
        tryagain:
        len = read(fd[0], buf, sizeof(buf));
        if(len == -1) {
            if(errno == EAGAIN) {
                write(STDOUT_FILENO, "try again\n", 10);
                sleep(1);
                goto tryagain;
            } else {
                perror("read");
                exit(1);
            }
        }
        write(STDOUT_FILENO, buf, len);
        putchar('\n');
        close(fd[0]);
    }else {
        perror("fork");
        exit(1);
    }
    return 0;
}