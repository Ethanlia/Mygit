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

#define QUECTEL_TIME_MAX_LEN 128

int main(int argc, char *argv[])
{
    int connectfd = -1;
    int connfd = -1;
    char buff[QUECTEL_TIME_MAX_LEN];
    char buff_msg[QUECTEL_TIME_MAX_LEN];

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    int n;
    connectfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(connectfd < 0)
    {
        printf("create socket error :%s %d",strerror(errno), errno);
        exit(0);
    }
    
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    client_addr.sin_port=htons(9981);
    
    if(bind(connectfd, (struct sockaddr *)(&client_addr), sizeof(client_addr)) == -1)
    {
        printf("bind error:%s %d",strerror(errno), errno);exit(0);
    }
    
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(9980);
    
    if(connect(connectfd, (struct sockaddr *)(&server_addr),sizeof(server_addr)) == -1)
    {
        printf("conect error:%s %d",strerror(errno), errno);
        exit(0);
    }else
    {
        printf("hello ,This is %d\n",server_addr.sin_port);
        snprintf( buff_msg, QUECTEL_TIME_MAX_LEN, "hello ,This is %d\r\n", server_addr.sin_port );
        send(connectfd,buff_msg,QUECTEL_TIME_MAX_LEN,0);
        
    }
    
    while( ( n = read(connectfd, buff, QUECTEL_TIME_MAX_LEN) ) )
    {
        buff[n] = 0;
        printf("%s\r\n",buff);
        
    }
    
    if(n < 0)
    {
        printf("recv error");
    }
    
    close(connectfd);
    return 0;
}