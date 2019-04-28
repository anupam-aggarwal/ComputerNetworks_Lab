#include <stdio.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int b = bind(sSock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(b < 0){
        printf("Binding Unsuccessfull %s\n",strerror(errno));
        exit(0);
    }

    int l = listen(sSock,MAXPENDING);

    struct sockaddr_in clientAddr;
    int clientLength;
    int clientSock;
    int i,count = 3;
    int pid;
    for( i = 0; i < count; i++){
        
        if ((pid=fork())==0) {
            clientSock = accept(sSock,(struct sockaddr*)&clientAddr,&clientLength);

            close(sSock);

            if(clientSock < 0){
                printf("Error in getting client socket\n");
                exit(0);
            }

            char rcvd[32];
            memset(rcvd,0,BUFSIZE);
            fflush(stdout);
            fflush(stdin);
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
            fflush(stdout);
            fflush(stdin);
            fgets(msg,BUFSIZE,stdin);
            int bytesSent = send(clientSock,msg,strlen(msg),0);
            if (bytesSent != strlen(msg)) {
                printf("Error in sending the data\n");
                exit(0);
            }
			close(clientSock);
            exit(0);
        }
        wait(NULL);
        close(clientSock);
        
    }
    

    close(sSock);

    return 0;
}
