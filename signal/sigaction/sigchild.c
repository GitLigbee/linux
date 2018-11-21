#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sys_exit(char* str, int no)
{
    perror(str);
    exit(no);
}

void do_sig(int signo)
{
    // while( (waitpid(0, NULL, WNOHANG)) != -1) ;

    int status;
    pid_t pid;

    while( (pid = waitpid(0, &status, WNOHANG)) > 0 ) {
        if( WIFEXITED(status) ) {
            printf("child %d exit %d\n", pid, WEXITSTATUS(status));
        } else if( WIFSIGNALED(status) ) {
            printf("child %d cancel signal %d\n", pid, WTERMSIG(status));
        }
    }
}

int main(void)
{
    pid_t pid;
    int i = 8;
    // 阻塞SIGCHLD

    while(i--) {
        if( (pid = fork()) == 0) {
            break;
        } else if( pid < 0) {
            sys_exit("fork", 1);
        }
    }

    if(pid == 0) {
        printf("child id: %d \n\n\n", getpid());
        sleep(20);
        return 1;
    }else {
        // 先设置捕捉
        // 再解除对SIGCHILD的阻塞
        struct sigaction act;
        act.sa_handler = do_sig;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, NULL);
        while(1) {
            printf("parent id:%d \n\n", getpid());
            sleep(2);
        }
    }
    return 0;
}