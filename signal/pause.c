#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void do_sig(int num)
{

}

int main(void)
{
    struct sigaction act;
    act.sa_handler = do_sig;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);

    pause();
    // kill -SIGUSR1 pid
    printf("over sleep\n");
    return 0;
}