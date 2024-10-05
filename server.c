#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define GUAN1 "set"
#define GUAN2 "get"
#define GUAN3 "delete"

typedef struct{
    char key[100];
    char value[100];
}ha;
void error_handling(char *message);
void chaozuo(char* p,int *num1,ha *arry);
int main(int argc,char *argv[])
{
    ha haxi[1000];
    int i=0,*num1;
    num1=&i;
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
        printf("%s",message1);
        if(strncmp(message1,"END",3)==0) break;
    chaozuo(message1,num1,haxi);
    if(a==-1){
        error_handling("read()error!");
        continue;
    }

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
void chaozuo(char*s,int *num1,ha *arry){
    char sh[100];
    char ch[100];
    int i=0,b=1;
    sscanf(s,"%[^(]",sh);
    if (!strcmp(GUAN1,sh))
    {
        if(sscanf(s,"set(%[^,],%[^)]",arry[*num1].key,arry[*num1].value)==2);
        write(4,"set success",sizeof("set success"));
        *num1+=1;
        printf("%d",*num1);
    }
    else if(!strcmp(GUAN2,sh))
    {
        for(;i<*num1;i++){
            sscanf(s,"get(%[^)])",ch);
            printf("%s\n",ch);
            printf("%s\n",arry[i].key);
            b=strcmp(ch,arry[i].key);
            if (b==0) break;
        }
        if(i==*num1) write(4,"Illegal Input",sizeof("Illegal Input")); //通信“Illegal Input”
        else write(4,arry[i].value,sizeof(arry[i].value));
    }
    else if(!strcmp(GUAN3,sh))
    {
        for(i=0;i<*num1;i++){
            sscanf(s,"get(%s)",ch);
            b=strcmp(ch,arry[i].key);
            if(b==0)break;
        }
        if(i==*num1){
        write(4,"Illegal Input",sizeof("Illegal Input"));
        }
        else{
        strcpy(arry[i].key,":),(o o),^_^,(:");
        write(4,"delete success",sizeof("delete success"));
        }
    }
    else write(4,"Illegal Input",sizeof("Illegal Input"));
}