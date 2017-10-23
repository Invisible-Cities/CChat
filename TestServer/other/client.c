#include "network_util.h"
#define PORT 31337
#define STDIN_FILENO 0

int main(int argc, char** argv) {

	if (argc != 4) {
		printf("Usage: ./client [IP] [PORT] [NAME]");
		exit(1);
	}
	fd_set fds;
	int maxfd;
	char buffer[500];

	char* ip = argv[1];
	int port = atoi(argv[2]);
	int yes = 1;
	char* name = argv[3];	
	printf("Port %d\n", port);

	int sockfd; //For connecting to the server
	struct sockaddr_in host_addr, server_addr; //Address info
	socklen_t sin_size;
	
	//Set up server memory header
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	
	if(inet_pton(AF_INET, ip, &server_addr.sin_addr.s_addr) == 0) {
		fatal("setting up server address (bad ip?)");
	}
	//Done setting up server address


	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		fatal("in socket");
	}
	maxfd = (sockfd > STDIN_FILENO) ? sockfd : STDIN_FILENO;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		fatal("setting socket option SO_REUSEADDR");
	}

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(PORT);
	host_addr.sin_addr.s_addr = 0;
	memset(&(host_addr.sin_zero), '\0', 8);

	if (bind(sockfd, (struct sockaddr*) &host_addr, sizeof(struct sockaddr)) == -1) {
		fatal("binding to socket");
	}

	size_t input_size;
	int buf_size;
	if (connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0 == -1) {
		fatal("failed to connect");
	}
	else {
		FD_ZERO(&fds);
		FD_SET(sockfd, &fds);
		FD_SET(STDIN_FILENO, &fds);
		printf("Connected!\n");
		while(1) {
			select(sockfd+1, &fds, NULL, NULL, NULL);
			if(FD_ISSET(STDIN_FILENO, &fds)) {
				printf("STDIN_FILENO INPUT RECEIVED\n");
				getline((char**) &(buffer), &input_size, stdin);
				printf("%s\n", buffer);
				send_string(sockfd, buffer);
			}
			if(FD_ISSET(sockfd, &fds)) {
				buf_size = recv_line(sockfd, buffer);
				printf("RECV: %d bytes\n",  buf_size);
				printf("%s", buffer);
				dump(buffer, buf_size);
			}
		}
	}
	close(sockfd);
	return 0;
}
	


