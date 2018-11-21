#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 0x10000

void* th_fun(void* argv)
{
    int n = 10;
    while(n--) {
        printf("%x  %d\n", (int)pthread_self(), n);
        sleep(1);
    }
    return (void *)1;
}

int main(void)
{
    pthread_t tid;
    pthread_attr_t attr;
    size_t stacksize;
    void* stackaddr;
    int err, detachstate;

    pthread_attr_init(&attr);
    // 获取栈大小
    pthread_attr_getstack(&attr, &stackaddr, &stacksize);
    printf("stackaddr = %p\n", stackaddr);
    printf("stacksize = %x\n", (int)stacksize);

    // 获取分离态
    pthread_attr_getdetachstate(&attr, &detachstate);
    if(detachstate == PTHREAD_CREATE_DETACHED) {
        printf("thread detached\n");
    } else if(detachstate == PTHREAD_CREATE_JOINABLE) {
        printf("thread joinable\n");
    } else {
        printf("thread unknown!\n");
    }
    // 设置栈分离态
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    // 设置栈大小
    stackaddr = malloc(SIZE);
    if( stackaddr == NULL) {
        perror("malloc");
        exit(1);
    }
    stacksize = SIZE;
    pthread_attr_setstack(&attr, &stackaddr, &stacksize);

    err = pthread_create(&tid, &attr, th_fun, NULL); 
    if(err != 0) {
        printf("%s\n", strerror(err));
        exit(1);
    }
    // pthread_join(tid, NULL);

    pthread_attr_destroy(&attr);
    return 0;
}