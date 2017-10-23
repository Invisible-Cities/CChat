#include "network_util.h"

#define PORT 6789

int main(void) {
	fd_set master;
	fd_set read_fds;
	int fdmax;
	int yes = 1;

	int listener_fd;
	int newfd; //Newly accepted socket descriptor
	struct sockaddr_in host_addr, client_addr;
	socklen_t addrlen;

	char buf[256];
	int i, j, rv;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	if ((listener_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");
	if (setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("setting socket option SO_REUSEADDR");

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(PORT);
	host_addr.sin_addr.s_addr = 0;
	memset( &(host_addr.sin_zero), '\0', 8);

	if(bind(listener_fd, (struct sockaddr*) &host_addr, sizeof(struct sockaddr)) == -1) {
		fatal("binding to socket");
	}
	if(listen(listener_fd, 5) == -1) {
		fatal("listening on socket");
	}

	FD_SET(listener_fd, &master);
	fdmax = listener_fd;
	/*addrlen = sizeof(struct sockaddr_in);
	newfd = accept(listener_fd, (struct sockaddr*) &client_addr, &addrlen);
	 if(newfd == -1) {
		fatal("accepting connection");
	}
	else {
		printf("server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	} */

	while(1) {
		read_fds = master; //Copy!
		if ( select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
			fatal("trying to select");
		}
		for( i=0 ; i < fdmax+1; i++) {
			if (FD_ISSET(i, &read_fds)) {
				if(i == listener_fd) {
					addrlen = sizeof(struct sockaddr_in);
					newfd = accept(listener_fd, (struct sockaddr*) &client_addr, &addrlen);

					if(newfd == -1) {
						fatal("accepting connection");
					}

					printf("server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				
					FD_SET(newfd, &master); //Add newfd to the list :)
					if(newfd > fdmax) {
						fdmax = newfd;
					}
				} else {
					printf("Incoming data\n");
					if (recv_line(i, buf) == -1) {
						printf("Error receiving data from client, connection closed\n");
						close(i);
						FD_CLR(i, &master);
					} else {
						for(j = 0; j <= fdmax;j++) {
							if (FD_ISSET(j, &master)) {
								if( j != listener_fd && j != i) {
									if(send_string(j, buf) == 0) {
										fatal("sending data");
									}
								}
							}
						}
					}
				} //Else for listner/outsider check
			}
		} //End first for loop
		memset(buf, '\0', 256);
	} //End while loop
	return 0;
}
										
				
