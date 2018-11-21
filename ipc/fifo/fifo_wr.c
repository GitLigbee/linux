#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void sys_exit(char* str, int exitno)
{
    perror(str);
    exit(exitno);
}

int main(int argc, char* argv[])
{
    int fd, len;
    char buf[64] = "hello fifo";
    if(argc < 2) {
        printf("./a.out fifoname");
        exit(1);
    }
    fd = open(argv[1], O_WRONLY);
    if(fd < 0) {
        sys_exit("open", 2);
    }
    write(fd, buf, strlen(buf));
    close(fd);
    exit(1);
}