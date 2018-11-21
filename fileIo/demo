#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int len;
    int fd_src, fd_dest;
    char buf[1024];

    if(argc < 3) {
        printf("./cp src dest\n");
        exit(-1);
    }

    fd_src = open(argv[1], O_RDONLY);
    fd_dest = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);

    while( (len = read(fd_src, buf, sizeof(buf))) > 0) {
        write(fd_dest, buf, len);
    }

    close(fd_src);
    close(fd_dest);
}