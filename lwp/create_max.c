#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void* th_fun(void *argv) {
    while(1) sleep(1);
}
// 测试一个进程可以创建线程个数
int main(void)
{
    pthread_t tid;
    int i= 1, err;

    while(1)
    {
        err = pthread_create(&tid, NULL, th_fun, NULL);
        if(err != 0) {
            printf("%s\n", strerror(err));
            exit(1);
        }
        printf("%d\n", i++);
    }
    return 0;
}