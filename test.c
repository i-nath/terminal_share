#include"test.h"
char * rstring(int );
int main()
{
    time_t t;
    /*t=time(NULL);
    ttime=localtime((const time_t*)&t);
    int lfd=open("log.txt",O_APPEND|O_CREAT,0660);
    if(lfd<0){
        perror("Log File");
    }
    if(logLevel>0)
    {
        write(lfd,asctime(localtime(&t)),strlen(asctime(localtime(&t))));
        //fsync(lfd);
        close(lfd);
    }*/
    int serverfd= socket(AF_INET,SOCK_STREAM,0);
    if(serverfd<0)
    {
        perror("Socket");
        return 0;
    }
    struct sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(8888);
    saddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    int eflag=1;
    if(setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&eflag,sizeof(int))<0)
    {
        perror("setsockopt");
        close(serverfd);
        return 0;
    }
    if(bind(serverfd,(const struct sockaddr *)&saddr,sizeof(saddr))<0)
    {
        perror("Bind");
        close(serverfd);
        return 0;
    }
    if(listen(serverfd,1)<0)
    {
        perror("listen");
        close(serverfd);
        return 0;
    }
    struct sockaddr_in caddr;//ipv4 client address only
    int clen=sizeof(caddr);
    puts("test1");
    int clientfd;
    while(1)//multi client server using fork
    {
        int pid;
        clientfd=accept(serverfd,(struct sockaddr *)&caddr,&clen);
        if((pid=fork())==-1)
        {
            perror("fork");
            close(clientfd);
            continue;
        }
        else if(pid == 0)
        {
            break;
        }
        else
        {
           close(clientfd);
            continue; 
        }
        
    }
    char *buff=NULL;//char pointer to point to client buffer 
    puts("test2");
    int i=0;
    while(1)//dynamic buffer recive from client 
    {
    buff=realloc(buff,i+1);
    recv(clientfd,buff+i,1,0);
    if(buff[i]==EOF || buff[i]==0)
    {
        break;
    }
    i++;
    }
    
        
    char fpath[40];//load the path where the files need to store , leave for same exicutable dir
    char fe[]=".txt";
     char *p;
    while(1)
    {
        p=rstring(5);
        strcat(fpath,p);
        strcat(fpath,fe);
        int dummyFd=open(fpath,O_RDONLY);//dummy fd to check the existing file with random string
        if(dummyFd==-1)//no file then return -1
        {
            close(dummyFd);//-1 break while and close dummyFd
            break;
        }
    }

    int filefd= open(fpath,O_RDWR|O_CREAT,0775);
    write(filefd,buff,strlen(buff));
    send(clientfd,"http://alpha1.myioat.com/",25,0);
    send(clientfd,p,strlen(p),0);
    send(clientfd,".txt",strlen(".txt"),0);
    send(clientfd,"\n",1,0);
    free(p);
    free(buff);
    close(filefd);
    close(clientfd);
    close(serverfd);    
    return 0;
}

//**********************************
/*randome string generator 
including small and captial letters
 and numbers 
 now only possible user option is length of string
 will add option to chose the custom letters and options for random chars in string
*/
char * rstring(int n )
{
    
  struct timeb t;
    ftime(&t);
    int seed=t.time+(1000)*t.millitm; 
    srand((getpid()+seed));
    char *rSPointer=NULL;
    char a[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    //printf("%f",t);
    rSPointer=(char*)malloc(n+1);
    int i;
    for(i=0;i<n;i++)
    {
        rSPointer[i]=a[(rand()%(sizeof(a)-1))];
    }
    rSPointer[i]=0;
    return rSPointer;
}