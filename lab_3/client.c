#include <stdio.h>
#include <sys/socket.h>
//for socket(), connect(), send(), recv() functions
#include <arpa/inet.h>
// different address structures are declared here
#include <stdlib.h>
// atoi() which convert string to integer
#include <string.h>
#include <unistd.h> // close() function
#define BUFSIZE 32

int main(){
    int sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock < 0){
        printf("Error in getting socket\n");
        exit(0);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int conn = connect(sock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(conn < 0){
        printf("Error in establishing a connection\n");
        exit(0);
    }

    printf("Enter message to be sent\n");
    fflush(stdout);
    fflush(stdin);
    char msg[BUFSIZE];
    memset(msg,0,BUFSIZE);
    fgets(msg,BUFSIZE,stdin);
    int bytesSent = send(sock,msg,strlen(msg),0);
    if (bytesSent != strlen(msg)) {
        printf("Error in sending the data\n");
        exit(0);
    }

    char rcvd[32];
    memset(rcvd,0,BUFSIZE);
    fflush(stdout);
    fflush(stdin);
    int bytesRcvd = recv(sock,rcvd,BUFSIZE-1,0);
    if(bytesRcvd != strlen(rcvd)){
        printf("Error in sending the data\n");
        exit(0);
    }
    rcvd[bytesRcvd] = '\0';

    printf("%s msg received\n",rcvd);

    close(sock);
    
    return 0;
    

}