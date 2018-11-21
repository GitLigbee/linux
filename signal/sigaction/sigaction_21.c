#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void doSig(int num)
{
    int index = 5;
    printf("i am handler\n");
    while(index--) {
        printf("signal num:%d\n", num);
        sleep(1);
    }
}

int main(void)
{
    struct sigaction act;
    act.sa_handler = doSig;
    
    act.sa_handler = doSig;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    
    while(1) {
        printf("------\n");
        sleep(1);
    }
    return 0;
}