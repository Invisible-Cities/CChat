/*
Defines various structs and creates various constants
*/

#define MAX_USERS 100
#define MAX_CHANNELS 100
#define MAX_CHANNEL_USERS 100

typedef struct user {
	int fd;
	Channel* curr_channel;
	struct sockaddr_in addr;
	char name[20];
} User;

typedef struct channel {
	int channel_id;
	int cur_users;
	User* users[MAX_CHANNEL_USERS];
	char name[20];
} Channel;
