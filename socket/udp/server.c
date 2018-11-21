#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>

#define SERVER_PORT 8000
#define MAX 1024

int main(void)
{
    int sockFd;
    struct sockaddr_in serverAddr, clientAddr;
    char buf[MAX];
    char ipstr[INET_ADDRSTRLEN];
    socklen_t clientLen;
    int i;
    ssize_t len;

    // 构造 ipv4 UDP通信套接字
    sockFd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);
    bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    while(1) {
        clientLen = sizeof(clientAddr);
        len = recvfrom(sockFd, buf, sizeof(buf), 0, (struct sockaddr*)&clientAddr, &clientLen);
        printf("client ip:%s\t port:%d\n", 
            inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ipstr, sizeof(ipstr)), ntohs(clientAddr.sin_port));
        i = 0;
        while(i < len) {
            buf[i] = toupper(buf[i]);
            i++;
        }
        buf[i] = '\0';
        sendto(sockFd, buf, len, 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    }
    
    close(sockFd);
    return 0;
}