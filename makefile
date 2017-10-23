INC=-I/home/Angel/Test/Chat/HEADERS

chat: server2.c HEADERS/*
	gcc -c $(INC) -o chat server2.c 
