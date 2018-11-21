#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>

#define SERVER_PORT 8000
#define MAX 4096

int main(void)
{
    struct sockaddr_in serverAddr, clientAddr;
    int socketFd, connectFd; // socket文件描述符
    socklen_t addrLen;
    char ipstr[128];
    char buf[MAX];
    int len, i;

    // socket
    socketFd = socket(AF_INET, SOCK_STREAM, 0); // ipv4 tcp
    // bind
    bzero(&serverAddr, sizeof(serverAddr));
    // 地址族协议 ipv4
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 本机任意IP地址
    serverAddr.sin_port = htons(SERVER_PORT);
    bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    // listen
    listen(socketFd, 128);
    printf("listen %d\n", SERVER_PORT);
    while(1) {
        // accept 阻塞监听客户端链接
        addrLen = sizeof(clientAddr);
        connectFd = accept(socketFd, (struct sockaddr *)&clientAddr, &addrLen);
        // 输出客户端IP：port
        // inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ipstr, sizeof(ipstr)); // 长度防止越界
        printf("client ip: %s\t port:%d\n", 
            inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ipstr, sizeof(ipstr)),
            ntohs(clientAddr.sin_port) );
        // 与客户端交互数据 read / write
        len = read(connectFd, buf, sizeof(buf));
        i = 0;
        while(i < len) {
            buf[i] = toupper(buf[i]);
            i++;
        }
        write(connectFd, buf, len);
        // close
        close(connectFd);
    }
    close(socketFd);

    return 0;
}