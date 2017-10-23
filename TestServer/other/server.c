#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6789 //Port the server will use

int main(int argc, char** args) {
	int sockfd, new_sockfd; //Listen on sock_fd, New connection on new_fd
	struct sockaddr_in host_addr, client_addr; //Address information
	socklen_t sin_size;
	
	int recv_length = 1, yes = 1;
	char buffer[1024];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) { //Stream is tcp, datapacket would be udp
		fatal("in socket");
	}
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("Setting socket option SO_REUSEADDR"); //Allow socket to bind to port even if port appears to be in use
	
	//Set host_addr info
	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(PORT); //host to network short, big endian
	host_addr.sin_addr.s_addr = 0; //Automatically fill with ip
	memset(&(host_addr.sin_zero), '\0', 8); //Zero the rest of the struct

	//Attempt to bind to socket
	if (bind(sockfd, (struct sockaddr*) &host_addr, sizeof(struct sockaddr)) == -1) {
		fatal("binding to socket");
	}
	
	//Attempt to listen to socket with backlog of 5
	if(listen(sockfd, 5) == -1) {
		fatal("listening on socket");
	}
	
	while(1) {
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr*) &client_addr, &sin_size);
		if(new_sockfd == -1) {
			fatal("accepting connection");
		}
		printf("server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		send(new_sockfd, "[Hello world!] ", 15, 0);
		recv_length = recv(new_sockfd, &buffer, 1024, 0);
		while(recv_length > 0) {
			printf("RECV: %d bytes\n", recv_length);
			dump(buffer, recv_length);
			recv_length = recv(new_sockfd, &buffer, 1024, 0);
		}
		return 0;
	}
}
