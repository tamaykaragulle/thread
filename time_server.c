#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include <pthread.h>
#define max 30
#define PORT 2100
void *runner (void *param);
int main(int argc, char *argv[])
{
    int sersoc,clisoc,conn,len,wri;
    
    pthread_t tid;
    pthread_attr_t attr;
    
    socklen_t clilen;
    struct sockaddr_in servaddr,cliaddr;

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    
    /* Creat the server socket */
    if((sersoc=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("Socket Error");
        exit(0);
    }

    /* Bind the server socket to a local address */
    if(bind(sersoc,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
    {
        perror("Bind Error");
        exit(0);
    }
    
    /* Be ready to listen to client connections */
    listen(sersoc,10);

    for(;;)        // while true
    {
        printf("Server is waiting for new connections...\n");
        clilen=sizeof(cliaddr);
        /* Block until a new connection request comes froma client */
        conn=(accept(sersoc,(struct sockaddr *)&cliaddr,&clilen));
        if (conn < 0)
        {
            perror ("Error on accept");
            exit (0);
        }
        printf ("A Client connected...\n");
        pthread_attr_init(&attr);
        
        pthread_create(&tid, &attr, runner, &conn);
        
        pthread_join(tid,NULL);
        
       
    }

    
    return 0;
}

void *runner (void *param)
{
    int wri;
    int conn= * (int *)param;
    time_t ticks;
    char str[max];
    /* Find current time and send back to client */
    ticks=time(NULL);
    sleep(10);
    strcpy(str,ctime(&ticks));
    wri=(write(conn,str,sizeof(str)));
    if(wri<0)
    {
        printf("Write Error");
        exit(0);
    }
    close(conn);
    pthread_exit(0);
}
