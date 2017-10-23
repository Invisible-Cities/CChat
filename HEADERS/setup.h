/*
Functions for setting up stuff
*/

#include "util.h"
#define QUEUE_SIZE 10

/*
port:		Port to bind to
host_addr:	struct to fill out
ret:		listening sockfd

Sets up [host_addr]) and returns fd of a socket listening on [port]
*/

int host_setup(int port, sockaddr_in* host_addr);
