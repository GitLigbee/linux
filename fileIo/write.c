#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    int fd;
    int ret;
    char str[16] = "helloWorld";

    fd = open("demo", O_CREAT | O_WRONLY, "0777");
    ret = write(fd, str, strlen(str));
    if(ret == -1) {
        printf("write fail\n");
    } else {
        printf("write succ, bytes:%d\n", ret);
    }
    close(fd);
    return 0;
}