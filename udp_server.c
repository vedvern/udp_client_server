#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>

#define SIZE 2048

int main(int argc, char **argv){
	
	struct sockaddr_in sa, ca;
	int sock_fd;
	int rec_len;
	int msg_count = 0;
	unsigned char buffer[SIZE];
	socklen_t addrlen = sizeof(ca);
	
	if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("\nFailed to create server socket\n");
		exit(EXIT_FAILURE);
	}
	
	printf("\nServer socket created successfull!!\n");
	
	memset((char *)&sa, 0, sizeof(sa));
	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(2050);
	
	if(bind(sock_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0){
		perror("\nBind failed!!\n");
                exit(EXIT_FAILURE);	
	}
	
	printf("\nBind successfull!!\n");

	while(1){
		printf("\nWaiting for data on port %d\n", htons(sa.sin_port));
		
				
		rec_len = recvfrom(sock_fd, buffer, SIZE, 0, (struct sockaddr *)&ca, &addrlen);
		if(rec_len > 0){
			buffer[rec_len] = 0;
			//printf("%d byte message received from client: %s", rec_len, buffer);
			printf("%d byte message received from %s port %d: %s", rec_len, inet_ntoa(ca.sin_addr), ntohs(ca.sin_port), buffer);
		}		
		else{
			printf("\nError Recieving from client!!\n");
		}
		
		sprintf(buffer, "\nAck message no: %d\n", msg_count++);
			
		printf("\nResponding to client\n");
		if(sendto(sock_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&ca, addrlen) < 0){
			perror("\nError sending to client!!\n");
		}
	}
}

