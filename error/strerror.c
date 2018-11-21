#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(void)
{
    int fd = open("file", O_WRONLY);
    if (fd < 0) {
        printf("msg: %s\n", strerror(errno));
    } else {
        close(fd);
    }
    printf("fd = %d\n", fd);
    return 0;
}
