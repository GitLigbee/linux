#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int fd;
    fd = open("demo.txt", O_CREAT, "0777");
    printf("fd = %d\n", fd);
    close(fd);
    return 0;
}