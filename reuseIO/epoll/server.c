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

#include <sys/epoll.h>

#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024
int main(int argc, char *argv[])
{
    int i, j, maxi, listenfd, connfd, sockfd;
    int nready, efd, res;
    ssize_t n;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t clilen;
    int client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;
    struct epoll_event tep, ep[OPEN_MAX];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 20);
    for (i = 0; i < OPEN_MAX; i++)
        client[i] = -1;
    maxi = -1;
    efd = epoll_create(OPEN_MAX);
    if (efd == -1) {
        fputs("epoll_create\n", stderr);
        exit(1);
    }
        
    tep.events = EPOLLIN;
    tep.data.fd = listenfd;
    res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
    if (res == -1) {
        fputs("epoll_ctl\n", stderr);
        exit(1);
    }
        
    for (;;)
    {
        nready = epoll_wait(efd, ep, OPEN_MAX, -1); /* 阻塞监听 */
        if (nready == -1){
            fputs("epoll_wait\n", stderr);
            exit(1);
        }
            
        for (i = 0; i < nready; i++)
        {
            if (!(ep[i].events & EPOLLIN))
                continue;
            if (ep[i].data.fd == listenfd)
            {
                clilen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
                printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));
                for (j = 0; j < OPEN_MAX; j++) {
                    if (client[j] < 0) {
                        client[j] = connfd; /* save descriptor */
                        break;
                    }
                }
                    
                if (j == OPEN_MAX) {
                    fputs("too many clients\n", stderr);
                    exit(1);
                }
                
                if (j > maxi)
                    maxi = j; /* max index in client[] array */
                tep.events = EPOLLIN; tep.data.fd = connfd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
                if (res == -1) {
                    fputs("epoll_ctl\n", stderr);
                    exit(1);
                }
            }
            else
            {
                sockfd = ep[i].data.fd;
                n = read(sockfd, buf, MAXLINE);
                if (n == 0)
                {
                    for (j = 0; j <= maxi; j++)
                    {
                        if (client[j] == sockfd)
                        {
                            client[j] = -1;
                            break;
                        }
                    }
                    res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    if (res == -1) {
                        fputs("epoll_ctl\n", stderr);
                        exit(1);
                    }
                    close(sockfd);
                    printf("client[%d] closed connection\n", j);
                }
                else
                {
                    for (j = 0; j < n; j++)
                        buf[j] = toupper(buf[j]);
                    write(sockfd, buf, n);
                }
            }
        }
    }
    close(listenfd);
    close(efd);
    return 0;
}