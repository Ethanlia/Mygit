#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>

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
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
        printf("create socket error :%s %d",strerror(errno), errno);
        exit(0);
    }
    
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(9980);
    
    // 绑定
    socklen_t  addrlen  = sizeof(struct sockaddr_in);
    if(bind(listenfd, (struct sockaddr *)(&server_addr), addrlen) == -1)
    {
        printf("bind error:%s %d",strerror(errno), errno);exit(0);            
    }
    
    if(listen(listenfd, 5) == -1)
    {
        printf("listen error:%s %d",strerror(errno), errno);
        exit(0);
    }

    connfd = accept(listenfd, (struct sockaddr *)&client_addr, &addrlen );
    if(connfd == -1)
    {
        printf("accept error:%s %d",strerror(errno), errno);
        exit(0);        
    }  
     //  当链接成功 则返回一个已经链接的套接字 
    printf("链接成功!!!\n");
    
    char * msg =  calloc(1, 128);

    while (1)
    {
        ticks = time(NULL);
        snprintf( buff, QUECTEL_TIME_MAX_LEN, "%s\r\n", ctime(&ticks) );
        printf("buff=%s\r\n",buff);

        // 把已连接套接字 + 标准输入描述符添加到 集合中
        fd_set set ; 
        FD_ZERO(&set);  // 清空集合
        FD_SET(connfd , &set); // 添加 套记字到集合中
        FD_SET(STDIN_FILENO , &set); // 添加标准输入到集合中

        // 找到描述符最大值
        int max_fd = connfd > STDIN_FILENO ? connfd : STDIN_FILENO ;

        struct timeval timeout = {
            .tv_sec = 20 ,
            .tv_usec = 0 
        }; // 设置时间结构体为20秒

        // 设置多路复用 监听标准输入以及已链接套接字   ， 超时设置为20秒
        int ret_val = select( max_fd + 1 , &set, NULL , NULL , &timeout);
        if (0 == ret_val)  // 超时
        {
            printf("超时警告！！！\n");
            continue ;
        }else if (-1 == ret_val) // 出错
        {
            perror("select error>>>>");
            continue ;
        }
        else{
            printf("有数据到达...\n");
        }

        // 检查哪个文件的数据到达
        if( FD_ISSET(connfd , &set))
        {
    
            ret_val = recv(connfd , msg , 128 , 0); // 从套接字获得数据并发送
            if (0 ==  ret_val)
            {
                printf("客户他，，，挂了》》》》\n");
                connfd = accept(listenfd , (struct sockaddr *)&client_addr, &addrlen);
                if ( -1 == connfd )
                {
                    perror("accept error>>>");
                    continue ; 
                }
                //  当链接成功 则返回一个已经链接的套接字 
                printf("链接成功!!!\n");
            }
            printf("msg:%s \n" , msg);
        }
        
        if( FD_ISSET(STDIN_FILENO , &set))
        {
            fgets(msg , 128 , stdin);  // 获取标准输入数据
            send(connfd , msg , strlen(msg), 0 ); // 发送数据
        }
       /*  recv(connfd,buff_msg,QUECTEL_TIME_MAX_LEN,0);
        printf("%s",buff_msg);   
        
        write(connfd, buff, strlen(buff));
        close(connfd);
        connfd = -1;   */                                                                                                                                                                                                
    }
    
    close(connfd);
    close(listenfd);
    return 0;
 
}