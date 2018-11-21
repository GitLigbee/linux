#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void* th_fun(void* argv)
{
    int n = 30;
    while(n--) {
        printf("%x  %d\n", (int)pthread_self(), n);
        sleep(1);
    }
    return (void *)1;
}

int main(void)
{
    pthread_t tid;
    pthread_attr_t attr; // attr为垃圾值
    int err;

    pthread_attr_init(&attr); // 保存创建线程的默认属性
    // int detachstate: PTHREAD_CREATE_DETACHED PTHREAD_CREATE_JOINABLE
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // 设置分离态
    pthread_create(&tid, &attr, th_fun, NULL);
    err = pthread_join(tid, NULL);
    while(1) {
        if(err!=0) {
            printf("%s\n", strerror(err));
            sleep(1);
            pthread_exit((void*)1);
        }
    }
    return 0;
}