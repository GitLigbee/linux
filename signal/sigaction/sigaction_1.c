#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void doSig(int num)
{
    printf("i am handler\n");
    printf("signal num:%d\n", num);
}

int main(void)
{
    struct sigaction act;
    act.sa_handler = doSig;
    // act.sa_handler = SIG_DFL;
    // act.sa_handler = SIG_IGN;
    
    act.sa_handler = doSig;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    
    while(1) {
        printf("------\n");
        sleep(1);
    }
    return 0;
}