#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAPLEN 0x1000

void sys_err(char *str, int exitno)
{
    perror(str);
    exit(exitno);
}

int main(int argc, char* argv[])
{
    char *mm;
    int fd, i=0;

    if(argc < 2) {
        printf("./app fileName");
        exit(1);
    }
    fd = open(argv[1], O_RDWR | O_CREAT, 0777);
    if(fd < 0) {
        sys_err("open", 1);
    }
    if(lseek(fd, MAPLEN -1, SEEK_SET) < 0) {
        sys_err("lseek", 2);
    }
    if(write(fd, "\0", 1) < 0) {
        sys_err("write", 3);
    }
    mm = mmap(NULL, MAPLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd); // 文件描述符与mmap无关联
    if(mm == MAP_FAILED) {
        sys_err("mmap", 4);
    }
    while(1) {
        sprintf(mm, "counter:%d", i++);
        sleep(1);
    }
    munmap(mm, MAPLEN);
    return 0;
}