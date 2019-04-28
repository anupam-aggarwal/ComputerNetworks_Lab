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

    
    printf("Guess a number b/w 1 to 6:\n");
    fflush(stdout);
    fflush(stdin);
    
    char msg[BUFLEN];
    memset(msg,0,BUFLEN);
    fgets(msg,BUFLEN-1,stdin);

    int bytesSent = sendto(sock,msg,strlen(msg),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
    if (bytesSent != strlen(msg)) {
        printf("Error in sending data %s",strerror(errno));
        exit(0);
    }

    char buf[BUFLEN];
    memset(buf,0,BUFLEN);
    fflush(stdout);
    fflush(stdin);

    int slen = sizeof(servAddr);
    int bytesRcvd = recvfrom(sock,buf,BUFLEN-1,0,(struct sockaddr*)&servAddr,&slen);
    if (bytesRcvd != strlen(buf)) {
        printf("Error in receiving data %s",strerror(errno));
        exit(0);
    }

    buf[bytesRcvd] = '\0';
    printf("%s\n",buf);
    
    close(sock);

    return 0;
}
