#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAXPENDING 5
#define BUFSIZE 32

int main(){

    int sSock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (sSock == -1) {
        printf("Error in getting server socket\n");
        exit(0);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int b = bind(sSock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(b < 0){
        printf("Binding Unsuccessfull\n");
        exit(0);
    }

    int l = listen(sSock,MAXPENDING);

    struct sockaddr_in clientAddr;
    int clientLength;
    int clientSock = accept(sSock,(struct sockaddr*)&clientAddr,&clientLength);

    if(clientSock < 0){
        printf("Error in getting client socket\n");
        exit(0);
    }

    char rcvd[32];
    memset(rcvd,0,BUFSIZE);
    int bytesRcvd = recv(clientSock,rcvd,BUFSIZE-1,0);
    if(bytesRcvd != strlen(rcvd)){
        printf("Error in sending the data\n");
        exit(0);
    }
    rcvd[bytesRcvd] = '\0';

    printf("%s msg received\n",rcvd);

    printf("Enter message to be sent\n");
    char msg[BUFSIZE];
    memset(msg,0,BUFSIZE);
    fgets(msg,BUFSIZE,stdin);
    int bytesSent = send(clientSock,msg,strlen(msg),0);
    if (bytesSent != strlen(msg)) {
        printf("Error in sending the data\n");
        exit(0);
    }

    close(clientSock);
    close(sSock);

    return 0;
}