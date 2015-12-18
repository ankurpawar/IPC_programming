all : server req1 req2 req3 calc1 calc2 calc3
server : server.o
	gcc server.o -o server
server.o : server.c
	gcc -c server.c
req1 : req1.c
	gcc req1.c -o req1
req2 : req2.c
	gcc req2.c -o req2
req3 : req3.c
	gcc req3.c -o req3
calc1 : calc1.c
	gcc calc1.c -o calc1
calc2 : calc2.c
	gcc calc2.c -o calc2
calc3 : calc3.c
	gcc calc3.c -o calc3
clean : server server.o req1 req2 req3 calc1 calc2 calc3
	rm -f server req1 req2 req3 calc1 calc2 calc3
	rm -f *.o
