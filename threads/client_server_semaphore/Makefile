all : server client1 client2 client3
server : server.o
	gcc server.o -g -o server -lpthread
server.o : server.c
	gcc -g -c server.c
client1 : client1.c
	gcc client1.c -o client1 -lpthread
client2 : client2.c
	gcc client2.c -o client2 -lpthread
client3 : client3.c
	gcc client3.c -o client3 -lpthread
clean :
	rm -f *.o server client1 client2 client3

	
