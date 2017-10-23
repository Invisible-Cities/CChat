#include "util.h"
#include "misc.h"
#include "setup.h"

int main(int argc, char** argv) {
	User user_list[MAX_USERS];
	Channel channel_list[MAX_CHANNELS];
	int sin_size;
	int total_users = 0;
	int total_channels = 0;
	
	int host_fd;
	sockaddr_in* host_addr;
	//First things first, set up the listening port
	
	host_fd = setup( atoi(argv[1]), host_addr);

	while(1) {
		sin_size = sizeof(struct sockaddr_in);
	
