INC=/home/Angel/Test/Chat/HEADERS
MAIN=server2.c

chat: $(MAIN) $(INC)/*
	gcc -c -I$(INC) -o chat $(MAIN)
