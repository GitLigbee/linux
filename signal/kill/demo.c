#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("my pid: %d\n", getpid());
    while(1) {}
    return 0;
}