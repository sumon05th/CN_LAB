#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<time.h>

#define MAXLINE 1024
    int main(int argc,char* argv[])
    {
    int sockfd;
    int n;
    socklen_t len;
    char sendline[1024],recvline[1024];
    struct sockaddr_in servaddr;
    strcpy(sendline,"");
    printf("\n Enter the message : ");

    clock_t begin=clock();

    scanf("%[^\n]",sendline);
    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=htons(5035);

    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    len=sizeof(servaddr);

    sendto(sockfd,sendline,MAXLINE,0,(struct sockaddr*)&servaddr,len);
    n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);

    recvline[n]=0;

    printf("\n Server's Echo : %s\n\n",recvline);
    clock_t end=clock();
    double time=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("Round Trip Time = %f\n",time);
    return 0;
    }