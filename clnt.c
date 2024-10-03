#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define SIZE1 1024
void error_handling(char *message);//定义了一个函数，void意味着不返回值，但是可以打印，可以改指针对应的值
int main(int argc,char* argv[])//argc表示输入参数数量，argv[]不太理解，argv[0]一般指向./文件名，空格分开，argv[1]下一个字符串
{
    char message0[SIZE1];
    char message1[SIZE1];
    int sock,num;//存储文件描述符
    struct sockaddr_in serv_addr;//(命名了一个结构体，该结构体在头文件中已声明)
    ssize_t str_len;
    if(argc!=3)//要三个参，运行，IP地址，端口（与服务器匹配）
    {
        printf("Usage : %s <IP> <port>\n",argv[0]);//报错暂时看不懂
        exit(1);//返回非零数给操作系统，终止关闭，返回0则正常退出
    }
    sock=socket(PF_INET,SOCK_STREAM,0);//PF_INET表示IPv4，type，0默认TCP
    if(sock==-1)//成文件描述符，败-1
        error_handling("socket()error");//调用error_handling函数
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("connect()error!");
    str_len=read(sock,message0,sizeof(message0)-1);
    if(str_len==-1)
        error_handling("read()error!");
    printf("message from server: %s",message0);
    while(1){
        fgets(message1,SIZE1,stdin);
        num=sizeof(message1);
        int* p=&num;
        write(sock,p,sizeof(num));
        write(sock,message1,sizeof(message1));
        if(strncmp(message1,"END",3)==0) break;
    }
    close(sock);
    return 0;
}
void error_handling(char *message)
{
    fputs(message,stderr);//和prinf有什么区别呢//用ctrl+Y撤回撤回
    fputc('\n',stderr);//
    exit(1);
}