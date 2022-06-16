#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <time.h>

#define  QUECTEL_TIME_MAX_LEN     128

int main(int argc, char *argv[])
{
    int listenfd = -1;
    int connfd = -1;
    char buff[QUECTEL_TIME_MAX_LEN];
    char buff_msg[QUECTEL_TIME_MAX_LEN];

    time_t ticks;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size;
    
    listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(listenfd < 0)
    {
        printf("create socket error :%s %d",strerror(errno), errno);
        exit(0);
    }
    
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(9980);
    
    if(bind(listenfd, (struct sockaddr *)(&server_addr), sizeof(server_addr)) == -1)
    {
        printf("bind error:%s %d",strerror(errno), errno);exit(0);            
    }
    
    if(listen(listenfd, 5) == -1)
    {
        printf("listen error:%s %d",strerror(errno), errno);
        exit(0);
    }
    
    while (1)
    {
        connfd = accept(listenfd, NULL, NULL );
        if(connfd == -1)
        {
            printf("accept error:%s %d",strerror(errno), errno);
            exit(0);        
        }  
        ticks = time(NULL);
        snprintf( buff, QUECTEL_TIME_MAX_LEN, "%s\r\n", ctime(&ticks) );
        printf("buff=%s\r\n",buff);

        recv(connfd,buff_msg,QUECTEL_TIME_MAX_LEN,0);
        printf("%s",buff_msg);   
        
        write(connfd, buff, strlen(buff));
        close(connfd);
        connfd = -1;                                                                                                                                                                                                  
    }
    
    return 0;
 
}