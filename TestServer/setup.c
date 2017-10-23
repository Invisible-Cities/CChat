/*
Functions for setting up stuff
*/

#include "util.h"
#include "setup.h"

/*
port:		Port to bind to
host_addr:	struct to fill out
ret:		listening sockfd

Sets up [host_addr]) and returns fd of a socket listening on [port]
*/

int host_setup(int port, sockaddr_in* host_addr) {
	//Set up the host_addr
	host_addr->sin_family = AF_INET;
	host_addr->sin_port = htons(port);
	host_addr->sin_addr->s_addr = 0;
	memset( &(host_addr->sin_zero), '\0', 8);

	//Set up the socket
	int sockfd;
	int yes = 1;
	socklen_t sin_size;
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		fatal("opening host socket");
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		fatal("setting socket option SO_REUSEADDR");
	}

	if(bind(sockfd, (struct sockaddr*) host_addr, &sin_size) == -1) {
		fatal("binding to socket");
	}

	if(listen(sockfd, QUEUE_SIZE) == -1)  {
		fatal("listening on socket")l
	}

	return sockfd;
}
