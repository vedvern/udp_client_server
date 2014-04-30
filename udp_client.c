#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/errno.h>

#define SIZE 2048

int main(int argc, char **argv){

	int sock_fd;
	struct sockaddr_in ca, sa;
	char buffer[SIZE];
	int rec_len;
	int s_len = sizeof(sa);
	char *server = "127.0.0.1";
	int i;
	
	if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("\nFailed to create client socket\n");
		exit(EXIT_FAILURE);
	}

	printf("\nClient socket created successfully!!\n");

	memset((char *)&ca, 0, sizeof(ca));
	ca.sin_family = AF_INET;
	ca.sin_addr.s_addr = htonl(INADDR_ANY);
	ca.sin_port = htons(0);
	
	memset((char *)&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(2050);

	if(inet_aton(server, &sa.sin_addr) == 0){
		perror("\nError!!\n");
		exit(EXIT_FAILURE);
	}
	
	printf("\nReady to transmit to %s port %d\n", server, htons(sa.sin_port));
	
	for(i=0; i<5; i++){
		printf("\nEnter the message: ");
		fgets(buffer, SIZE, stdin);
			
		if(sendto(sock_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&sa, s_len) == -1){
			perror("\nError sending data to server!!\n");
                	exit(EXIT_FAILURE);
		}
		
		rec_len = recvfrom(sock_fd, buffer, SIZE, 0, (struct sockaddr *)&sa, &s_len);
		if(rec_len >= 0){
			buffer[rec_len] = 0;
			printf("\n%s\n", buffer);
		}
	}
	close(sock_fd);
	return 0;
}
