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

int main(int argc, char* argv[])
{
    int i, maxi, maxfd, listenfd, confd, sockfd;
    int nready, client[FD_SETSIZE]; // FD_SETSIZE 默认为1024
    ssize_t n;
    fd_set rset, allset;
    char buf[MAX];
    char str[INET_ADDRSTRLEN]; // #define INET_ADDRSTRLEN 16
    socklen_t cliaddr_len;
    struct sockaddr_in cliaddr, serverAddr;

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

    maxfd = listenfd; // 初始化
    maxi = -1; // client[]的下标

    for(i =0; i< FD_SETSIZE; i++) {
        client[i] = -1; // 初始化client
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for(; ;) {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(nready < 0) {
            perror("select error");
            exit(1);
        }

        // 判断是否新的客户端链接
        if(FD_ISSET(listenfd, &rset) ) {
            cliaddr_len = sizeof(cliaddr);
            confd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);

            printf("client ip: %s\t port:%d\n", 
            inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, str, sizeof(str)),
            ntohs(cliaddr.sin_port) );

            for(i =0; i< FD_SETSIZE; i++) {
                if(client[i] < 0) {
                    client[i] = confd; // 保存accept返回的文件描述符到client
                    break;
                }
            }
            // 判断是否达到select能监控的文件个数上限 1024
            if(i == FD_SETSIZE) {
                fputs("too many clients\n", stderr);
                exit(1);
            }
            FD_SET(confd, &allset);
            if(confd > maxfd) {
                maxfd = confd;
            }
            if(i > maxi) {
                maxi = i;
            }
            if(--nready == 0) {
                continue; // 如果没有更多的文件描述符继续回到select阻塞监听，负责处理未处理完的就绪文件描述符
            }
        }

        // 存在数据传输
        for(i= 0; i<= maxi; i++) {
            if( (sockfd = client[i]) < 0) {
                continue;
            }
            if(FD_ISSET(sockfd, &rset)) {
                if( (n = read(sockfd, buf, MAX)) == 0) {
                    // 客户端已关闭链接，服务端也关闭对应链接
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    int j;
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
}
