#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);
int main(int argc,char *argv[])
{
    char guan1[]={"set"};
    char guan2[]={"get"};
    char guan3[]={"delete"};
    int serv_sockfd;
    int clnt_sockfd;
    int num;
    int* p=&num;
    ssize_t a;
    char message0[]=
    {"约定输入set(key,value)给数据库中键为key的属性赋予值value;get(key):返回key的值;delete(key)删除;若操作无效返回Ilegal Input,END退出"};
    char message1[100];
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    if(argc!=2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    serv_sockfd=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sockfd== -1)
        error_handling("socket() error");
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
    if(bind(serv_sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind() error");
    if(listen(serv_sockfd,5)==-1)
        error_handling("listen() error");
    clnt_addr_size=sizeof(clnt_addr);
    clnt_sockfd=accept(serv_sockfd,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sockfd==-1)
        error_handling("accept() error");
    write(clnt_sockfd,message0,sizeof(message0)); 
    while(1){
        a=read(clnt_sockfd,p,sizeof(num));
        a+=read(clnt_sockfd,message1,num);
        if(strncmp(message1,"END",3)==0) break;
    if(a==-1)
        error_handling("read()error!");
    printf("message from client: %s",message1);
    }

    close(clnt_sockfd);
    close(serv_sockfd);
    return 0;
}
void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
