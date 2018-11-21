#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if(argc < 3) {
        printf("./app pid signal\n");
        exit(1);
    }
    if( kill( (pid_t)atoi(argv[1]), atoi(argv[2]))) {
        perror("kill");
        exit(2);
    }
    return 0;
}