#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
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

    int b = bind(sock,(struct sockaddr*)&servAddr,sizeof(servAddr));
    if (b < 0) {
        printf("Error in binding socket %s",strerror(errno));
        exit(0);
    }

    struct dataPkt data;
    struct ackPkt ack;

    int state = 0;

    struct sockaddr_in clientAddr;
    int clientLen = sizeof(clientAddr);
    int bytesRcvd, bytesSent;
    while(1){
        switch (state)
        {
            case 0:
                fflush(stdout);
                fflush(stdin);
                memset(&data,0,sizeof(data));
                bytesRcvd = recvfrom(sock,&data,sizeof(data),0,(struct sockaddr*)&clientAddr,&clientLen);
                
                if (rand()%2 == 0) {
                    // discard the packet and receive it once again
                    printf("Data packet Lost\n");
                    fflush(stdout);
                    fflush(stdin);
                    bytesRcvd = recvfrom(sock,&data,sizeof(data),0,(struct sockaddr*)&clientAddr,&clientLen);
                    
					if (data.seq == 0) {
                        printf("Message received %s\n",data.msg);
                    }else{
                        fflush(stdout);
                		fflush(stdin);
                    	ack.seq = 1;
                    	bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
                   		if(bytesSent > 0){
                        	printf("ACK1 sent again\n");
                        	state = 0;
                    	}
						break;
                    }

                    ack.seq = 0;
                    fflush(stdout);
                	fflush(stdin);
                    bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
                    if(bytesSent > 0){
                        printf("ACK0 sent\n");
                        state = 1;
                    }

                }else{
                    if (data.seq == 0) {
                        printf("Message received %s\n",data.msg);
                    }else{
                        fflush(stdout);
                		fflush(stdin);
                   		ack.seq = 1;
                    	bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
                    	if(bytesSent > 0){
                        	printf("ACK1 sent again\n");
                        	state = 0;
                    	}
						break;
                    }

                    fflush(stdout);
                	fflush(stdin);
                    ack.seq = 0;
                    bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
                    if(bytesSent > 0){
                        printf("ACK0 sent\n");
                        state = 1;
                    }
                }
                
                
                break;
            
            case 1:
                fflush(stdout);
                fflush(stdin);
                memset(&data,0,sizeof(data));
                bytesRcvd = recvfrom(sock,&data,sizeof(data),0,(struct sockaddr*)&clientAddr,&clientLen);
                
                if (rand()%2 == 0) {
                    // discard the packet and receive it once again
                    printf("Data packet Lost\n");
                    fflush(stdout);
                	fflush(stdin);
                    bytesRcvd = recvfrom(sock,&data,sizeof(data),0,(struct sockaddr*)&clientAddr,&clientLen);
                    if (data.seq == 1) {
                        printf("Message received %s\n",data.msg);
                    }else{
                        fflush(stdout);
		            	fflush(stdin);
		                ack.seq = 0;
		                bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
		                if(bytesSent > 0){
		                    printf("ACK0 sent again\n");
		                    state = 1;
		                }
						break;
                    }
                    fflush(stdout);
                	fflush(stdin);
                    ack.seq = 1;
                    bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
                    if(bytesSent > 0){
                        printf("ACK1 sent\n");
                        state = 0;
                    }
                    
                }else{
                    if (data.seq == 1) {
                        printf("Message received %s\n",data.msg);
                    }else{
                        fflush(stdout);
                		fflush(stdin);
		                ack.seq = 0;
		                bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
		                if(bytesSent > 0){
		                    printf("ACK0 sent again\n");
		                    state = 1;
		                }
						break;
                    }
                    fflush(stdout);
                	fflush(stdin);
                    ack.seq = 1;
                    bytesSent = sendto(sock,&ack,sizeof(ack),0,(struct sockaddr*)&clientAddr,clientLen);
                    if(bytesSent > 0){
                        printf("ACK1 sent\n");
                        state = 0;
                    }
                }
                break;
        
        }
    }
    

    close(sock);

    return 0;
}
