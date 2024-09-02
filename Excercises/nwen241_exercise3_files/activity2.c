#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 23456
#define BUFFER_SIZE 1024


int main(){

	char buffer[BUFFER_SIZE] = {0};


	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		printf("Error creating socket");
		exit(0);
	} 


	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT); 
	addr.sin_addr.s_addr = INADDR_ANY; // any address


	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr))<0) {
		printf("Error binding socket");
		exit(0);
	}

	if(listen(fd, SOMAXCONN) < 0) {
		printf("Error listening for connections");
		exit(0);
	}

	struct sockaddr_in client_addr;
	int addrlen = sizeof(client_addr);
	int client_fd = accept(fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
	if(client_fd < 0) {
		printf("Error accepting connection");
		exit(0);
	}


	char incoming[100];
	ssize_t r = recv(client_fd, buffer, BUFFER_SIZE, 0);
	if(r <= 0) {
		printf("Error receiving message");
		close(client_fd);
		exit(0);
	}


	int length = strlen(buffer);
    for (int i = 0; i < length / 2; ++i) {
        char temp = buffer[i];
        buffer[i] = buffer[length - i - 1];
        buffer[length - i - 1] = temp;
    }


     send(client_fd, buffer, strlen(buffer), 0);

      
     close(client_fd);


}