#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define GUAN1 "set"
#define GUAN2 "get"
#define GUAN3 "delete"
#define GUAN4 "SAVE"
#define GUAN5 "addr"
#define GUAN6 "addl"
#define GUAN7 "index"

struct Node{
    char data[100];
    struct Node* left;
    struct Node* right;
};

typedef struct{
    int size;
    struct Node* firstnode;
    struct Node* lastnode;
}List;
struct Node* createnode(char* data){
    struct Node* newnode =(struct Node*)malloc(sizeof(struct Node));
    strncpy(newnode->data,data,sizeof(data)-1);
    newnode->left =newnode->right=NULL;
    return newnode;
}
List* createlist(void){
    List* list=(List*)malloc(sizeof(List));
    list->size=0;
    list->firstnode=list->lastnode=NULL;
    return list;
}
void chaleft(List* list,char* data){
    struct Node* newnode=createnode(data);
    if(list->firstnode==NULL){
        list->lastnode=newnode;
    }
    else{
        list->firstnode->left=newnode;
        newnode->right=list->firstnode;
        
    }
    strncpy(newnode->data,data,sizeof(data)-1);
    list->firstnode=newnode;
    list->size++;
}
void charight(List* list,char* data){
    struct Node* newnode=createnode(data);
    if(list->lastnode==NULL){
        list->firstnode=newnode;
    }
    else {
        list->lastnode->right=newnode;
        newnode->left=list->lastnode;
    }
    strncpy(newnode->data,data,sizeof(data)-1);
    list->lastnode=newnode;
    list->size++;
}
void printlist(List* list,int n){
    struct Node* p=list->firstnode;
    while(n>=-1){
        p=p->left;
        n--;
    }
    printf("%s",p->data);
    
}
typedef struct{
    char key[100];
    char value[100];
}ha;
void error_handling(char *message);
/*int save_but_txt(int i,char* haxi){
    FILE* file =fopen("/wsl.localhost/Ubuntu/home/powerli/linux_test/.vscode/data.txt","w+");
    if(file==NULL) 
    {
        write(4,"save failed",sizeof("save failed"));
        return 1;
    }
        for(int q=0;q<i;q++){
            fwrite(haxi[q],sizeof(haxi[q]),strlen(haxi[q]),file);
        }
        fclose(file);
        return 0;
    }

void time_set_save(int time,int i,int n1,char* haxi){
    while(1){
    if (n1<=n1){
    save_but_txt(i,haxi);
    sleep(time);
    }
    }
}
*/
void center(char* s,int* num1){
    char sh[1000];
    sscanf(s,"%[^(]",sh);
    if (!strcmp(GUAN1,sh)) *num1=0;//set0
    else if(!strcmp(GUAN2,sh)) *num1=1;//get1
    else if(!strcmp(GUAN3,sh)) *num1=2;//delete2
    else if(!strcmp(GUAN4,s)) *num1=3;//save3
    else if(!strcmp(GUAN4,sh)) *num1=4;//save(a,b)4
    else if(!strcmp(GUAN5,sh)) *num1=5;//addr(key,value)5
    else if(!strcmp(GUAN6,sh)) *num1=6;//addl()6
    else if(!strcmp(GUAN7,sh)) *num1=7;//index()6
    else write(4,"error",sizeof("error"));
}

int main(int argc,char *argv[])
{
    List* l;
    l=createlist();
    List list[100];
    ha haxi[10000];
    char key[100];
    char value[100];
    char ch[100];
    int i=0,*num1=NULL;
    int serv_sockfd;
    int clnt_sockfd;
    int num;
    int* p=&num;
    ssize_t a;
    char message0[]=
    {"约定输入set(key,value)给数据库中键为key的属性赋予值value;get(key):返回key的值;delete(key)删除;addr(key,value);addl(key,value);若操作无效返回Ilegal Input,END退出"};
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
        center(message1,num1);
        switch (*num1){
            case 0://set
                if(sscanf(message1,"set(%[^,],%[^)]",haxi[i].key,haxi[i].value)==2){
                write(4,"set success",sizeof("set success"));
                i++;
                }
                else write(4,"set failed",sizeof("set failed"));
            break;
            case 1://get
                int q=0;
                sscanf(message1,"get(%[^)])",ch);
                for(q=0;q<i;q++){
                int b=strcmp(ch,haxi[q].key);
                if (b==0) break;
                }
                if(q==i) write(4,"Illegal Input",sizeof("Illegal Input")); //通信“Illegal Input”
                else write(4,haxi[q].value,sizeof(haxi[q].value));
            break;
            case 2://delete
                sscanf(message1,"get(%s)",ch);
                for(int q=0;q<i;q++){
                int b=strcmp(ch,haxi[q].key);
                if (b==0) break;
                }
                if(q==i){
                write(4,"Illegal Input",sizeof("Illegal Input"));
                }
                else{
                strcpy(haxi[q].key,":),(o o),^_^,(:");
                write(4,"delete success",sizeof("delete success"));
                }
            break;
 /*         case 3://save
                int jian=save_but_txt(i,haxi);
                if(jian=0) write(4,"save success",sizeof("save success"));
                else write(4,"save failed",sizeof("save failed"));
            break;
             case 4:
                typedef struct{
                int time,i,n1;
                char* haxi;
                }shuru;
                shuru mn;
                mn.i=i;
                sscanf(message1,"save(%[^,],%[^)]",mn.time,mn.n1);
                pthread_t thread_id;
                pthread_create(&thread_id,mn,&save_but_txt);
*/
            case 5:
                if(sscanf(message1,"addr(%[^,],%[^)]",key,value)==2){
                write(4,"addr success",sizeof("addr success"));
                charight(l,value);
                }
                else write(4,"addr failed",sizeof("addr failed"));
            break;
            case 6:
                if(sscanf(message1,"addl(%[^,],%[^)]",key,value)==2){
                write(4,"addl success",sizeof("addr success"));
                chaleft(l,value);
                }
                else write(4,"addr failed",sizeof("addr failed"));
            break;
            default:
                write(4,"Illegal Input",sizeof("Illegal Input"));
            break;
            }

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
/*
        else{
        pid_t pid =fork();
        if(pid <0){
            //通信失败
            return 1;
        }
        else if(pid==0) {//子
            //.odb
        }else {//父

        int time,num3;
        
        if(sscanf(s,"%d%d",&time,&num3)==2){
            printf("自动触发");
}
*/
