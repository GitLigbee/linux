#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#define SERVER_PORT 8000
#define MAX 80
#define OPEN_MAX 1024

int main(int argc, char* argv[])
{
    int i,j, maxi, listenfd, confd, sockfd;
    int nready; // FD_SETSIZE 默认为1024
    ssize_t n;
    fd_set rset, allset;
    char buf[MAX];
    char str[INET_ADDRSTRLEN]; // #define INET_ADDRSTRLEN 16
    socklen_t cliaddr_len;
    struct sockaddr_in cliaddr, serverAddr;

    struct pollfd client[OPEN_MAX];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serverAddr, sizeof(serverAddr));
    // 地址族协议 ipv4
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 本机任意IP地址
    serverAddr.sin_port = htons(SERVER_PORT);
    bind(listenfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    // listen
    listen(listenfd, 128);
    printf("listen %d\n", SERVER_PORT);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM; // listenfd 监听普通读事件
    for(i= 1; i< OPEN_MAX; i++) {
        client[i].fd = -1;
    }
    maxi = 0;

    for(; ;) {
        nready = poll(client, maxi+1, -1); // 阻塞

        if(client[0].revents & POLLRDNORM) { // 客户端链接请求
            cliaddr_len = sizeof(cliaddr);
            confd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);

            printf("client ip: %s\t port:%d\n", 
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, str, sizeof(str)),
            ntohs(cliaddr.sin_port) );

            for(i= 1; i< OPEN_MAX; i++) {
                if(client[i].fd < 0) { // 找到client[]中的空闲位置，存放accept返回的connfd
                    client[i].fd = confd;
                    break;
                }
            }
            if(i== OPEN_MAX) {
                fputs("too many clients\n", stderr);
                exit(1);
            }
            client[i].events = POLLRDNORM;// 设置刚刚返回的connfd，监听读事件
            if(i > maxi) { // 更新client[]中最大元素下标
                maxi = i;
            }
            if(--nready == 0) { // 没有更多的就绪事件时，继续回到poll阻塞
                continue;
            }
        }

        for(i=1; i< maxi; i++) { // 检查client[]
            if((sockfd = clent[i].fd) < 0 ) {
                continue;
            }
            if(clent[i].revents & (POLLRDNORM | POLLERR)) {
                if( (n = read(sockfd, buf, MAX)) < 0) {
                    if( errno == ECONNRESET) {
                        // connection reset by client
                        printf("client[%d] aborted connection.\n", i);
                        close(sockfd);
                        client[i].fd = -1;
                    } else {
                        fputs("read error\n", stderr);
                        exit(1);
                    }
                } else if(n == 0) {
                    // connection closed by client
                    printf("client[%d] closed connection.\n", i);
                    close(sockfd);
                    clent[i].fd = -1;
                } else {
                    for(j= 0; j< n; j++) {
                        buf[j] = toupper(buf[j]);
                    }
                    write(sockfd, buf, n);
                }
                if(--nready == 0) {
                    break;
                }
            }
        }
    }
    close(listenfd);
}
