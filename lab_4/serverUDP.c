#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<errno.h>
#include<unistd.h>

#define BUFLEN 512 //Max length of buffer

int main(){
    
    int sock = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sock < 0){
        printf("Error in getting socket %s",strerror(errno));
        exit(0);
    }

    struct sockaddr_in servAddr;
    memset(&servAddr,0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(12345);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int b = bind(sock,(struct sockaddr*)&servAddr,sizeof(servAddr));
    if (b < 0) {
        printf("Error in binding socket %s",strerror(errno));
        exit(0);
    }

    int n = rand()%6 + 1;
    
    struct sockaddr_in clientAddr;
    int clientLen = sizeof(clientAddr);

    char buf[BUFLEN];
    memset(buf,0,BUFLEN);
    fflush(stdout);
    fflush(stdin);

    int bytesRcvd = recvfrom(sock,buf,BUFLEN-1,0,(struct sockaddr*)&clientAddr,&clientLen);
    if (bytesRcvd != strlen(buf)) {
        printf("Error in receiving data %s",strerror(errno));
        exit(0);
    }
    buf[bytesRcvd-1] = '\0';
    printf("%d temp\n",atoi(buf));

    char msg[512]= "You lost the game\n";
    fflush(stdout);
    fflush(stdin);
    int bytesSent = sendto(sock,msg,strlen(msg),0,(struct sockaddr*)&clientAddr,clientLen);
    if (bytesSent != strlen(msg)) {
        printf("Error in sending data %s",strerror(errno));
        exit(0);
    }

    close(sock);

    return 0;
}