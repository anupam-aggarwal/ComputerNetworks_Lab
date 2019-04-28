#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/time.h>
#include<errno.h>
#include<unistd.h>
#define BUFLEN 512 //Max length of buffer

struct dataPkt{
    char msg[BUFLEN];
    int seq;
};

struct ackPkt{
    int seq;
};



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

    struct dataPkt data;
    struct ackPkt ack;

    int state = 0;
    int bytesRcvd,bytesSent;
    int servLen = sizeof(servAddr);

    fd_set rfds;
    struct timeval tv;
    int retVal;



    while(1){
        switch (state)
        {
            case 0:
                fflush(stdout);
                fflush(stdin);
                printf("Waiting for message from user\n");
                memset(data.msg,0,BUFLEN);
                fgets(data.msg,BUFLEN,stdin);
                data.seq = 0;
                bytesSent = sendto(sock,&data,sizeof(data),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
                if (bytesSent < 0) {
                    printf("Error in sending data %s",strerror(errno));
                    exit(0);
                }
                state = 1;
                printf("Message sent\n");

                break;
            
            case 1:
                FD_ZERO(&rfds);
                FD_SET(sock,&rfds);
                tv.tv_sec = 10;
                tv.tv_usec = 0;
                printf("Wating for ACK 0\n");
                retVal = select(sock+1,&rfds,NULL,NULL,&tv);

                if (retVal < 0) {
                    
                }else if(retVal == 0){
                    fflush(stdout);
                	fflush(stdin);
                    bytesSent = sendto(sock,&data,sizeof(data),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
                    if (bytesSent < 0) {
                        printf("Error in sending data %s",strerror(errno));
                        exit(0);
                    }
                    //tv.tv_sec = 10;
                    //tv.tv_usec = 0;
                    break;
                }else{
                    fflush(stdout);
                    fflush(stdin);
                    
                    memset(&ack,0,sizeof(ack));
                    bytesRcvd = recvfrom(sock,&ack,sizeof(ack),0,(struct sockaddr*)&servAddr,&servLen);
                    
                    if (rand()%2 == 0) {
                        // discard ack pkt and resent the data pkt
                        printf("ACK Pkt lost\n");
                        fflush(stdout);
                		fflush(stdin);
                    	bytesSent = sendto(sock,&data,sizeof(data),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
		                if (bytesSent < 0) {
		                    printf("Error in sending data %s",strerror(errno));
		                    exit(0);
		                }
                    
                    	break;
                    }else{
                        if(ack.seq == 0){
                            printf("Received ack 0\n");
                            state = 2;
                        }
                    }
                    
                    
                    
                }
                

                
                break;
            
            case 2:
                fflush(stdout);
                fflush(stdin);
                printf("Waiting for message from user\n");
                memset(data.msg,0,BUFLEN);
                fgets(data.msg,BUFLEN,stdin);
                data.seq = 1;
                bytesSent = sendto(sock,&data,sizeof(data),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
                if (bytesSent < 0) {
                    printf("Error in sending data %s\n",strerror(errno));
                    exit(0);
                }
                state = 3;
                printf("Message sent\n");
                break;
            
            case 3:
                FD_ZERO(&rfds);
                FD_SET(sock,&rfds);
                tv.tv_sec = 10;
                tv.tv_usec = 0;
                printf("Wating for ACK 1\n");
                retVal = select(sock+1,&rfds,NULL,NULL,&tv);

                if (retVal < 0) {
                    
                }else if(retVal == 0){
                    fflush(stdout);
                	fflush(stdin);
                    bytesSent = sendto(sock,&data,sizeof(data),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
                    if (bytesSent < 0) {
                        printf("Error in sending data %s",strerror(errno));
                        exit(0);
                    }
					printf("Data Sent again\n");
                    //tv.tv_sec = 10;
                    //tv.tv_usec = 0;
                    break;
                }else{
                    fflush(stdout);
                    fflush(stdin);
                    memset(&ack,0,sizeof(ack));
                    bytesRcvd = recvfrom(sock,&ack,sizeof(ack),0,(struct sockaddr*)&servAddr,&servLen);
                    
                    if (rand()%2 == 0) {
                        // discard ack pkt and resent the data pkt
                        printf("ACK Pkt lost\n");
                        fflush(stdout);
                		fflush(stdin);
		                bytesSent = sendto(sock,&data,sizeof(data),0,(struct sockaddr*)&servAddr,sizeof(servAddr));
		                if (bytesSent < 0) {
		                    printf("Error in sending data %s",strerror(errno));
		                    exit(0);
		                }
						printf("Data Sent again\n");
                    
                    	break;
                    }else{
                        if(ack.seq == 1){
                            printf("Received ack 1\n");
                            state = 0;
                        }
                    }
                    
                }
        
        }
    }
    
    
    
    close(sock);

    return 0;
}
