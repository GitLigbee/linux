#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc < 2) {
        printf("./app fileName\n");
    }
    if(access(argv[1], F_OK) < 0) {
        perror(argv[1]);
        exit(1);
    }
    printf("%s is exist\n", argv[1]);
    return 0;
}