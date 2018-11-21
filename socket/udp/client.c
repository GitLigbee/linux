#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define SERVER_PORT 8000
#define MAX 4096

int main(int argc, char* argv[])
{
    struct sockaddr_in serverAddr;
    int connectFd;
    char ipstr[] = "192.168.1.101";
    char buf[MAX];
    ssize_t len;
    if(argc < 2) {
        printf("./client str\n");
        exit(1);
    }
    // 创建socket
    connectFd = socket(AF_INET, SOCK_DGRAM, 0);
    // 初始化服务器地址
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipstr, &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(SERVER_PORT);
    // 向服务器发送数据
    sendto(connectFd, argv[1], strlen(argv[1]), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    len = recvfrom(connectFd, buf, sizeof(buf), 0, NULL, 0);
    write(STDOUT_FILENO, buf, len);
    putchar('\n');
    // 关闭socket
    close(connectFd);
    return 0;
}