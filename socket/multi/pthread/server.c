#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLINE 80
#define SERV_PORT 8000

struct s_info {
    struct sockaddr_in cliaddr;
    int connfd;
};

void *do_work(void *arg)
{
    int n,i;
    struct s_info *ts = (struct s_info*)arg;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];

    // 创建线程设置分离态
    pthread_detach(pthread_self());
    while(1) {
        n = read(ts->connfd, buf, MAXLINE);
        if(n == 0) {
            printf("the other side has been closed.\n");
            break;
        }
        printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &(*ts).cliaddr.sin_addr, str, sizeof(str)), ntohs((*ts).cliaddr.sin_port));
        for(i = 0; i< n; i++) {
            buf[i] = toupper(buf[i]);
        }
        write(ts->connfd, buf, n);
    }
    close(ts->connfd);
}

int main(void)
{
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    int i=0;
    pthread_t tid;
    struct s_info ts[383];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // bind
    bzero(&serverAddr, sizeof(serverAddr));
    // 地址族协议 ipv4
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 本机任意IP地址
    serverAddr.sin_port = htons(SERVER_PORT);
    bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    // listen
    listen(socketFd, 128);
    printf("listen %d\n", SERV_PORT);

    while(1) {
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);
        ts[i].connfd = connfd;
        ts[i].cliaddr = cliaddr;
        pthread_create(&tid, NULL, do_work, (void*)&ts[i]);
        i++;
    }
    return 0;
}