all : server req1 req2 req3 calc1 calc2 calc3 
server : server.o
	gcc server.o -o server
server.o : server.c
	gcc -c server.c
req1 : req1.o
	gcc req1.o -o req1
req1.o : req1.c
	gcc -c req1.c
req2 : req2.o
	gcc req2.o -o req2
req2.o : req2.c
	gcc -c req2.c
req3 : req3.o
	gcc req3.o -o req3
req3.o : req3.c
	gcc -c req3.c
calc1 : calc1.o
	gcc calc1.o -o calc1
calc1.o :calc1.c
	gcc -c calc1.c
calc2 : calc2.o
	gcc calc2.o -o calc2
calc2.o :calc2.c
	gcc -c calc2.c
calc3 : calc3.o
	gcc calc3.o -o calc3
calc3.o : calc3.c
	gcc -c calc3.c
clean :
	rm -f *.o server req1 req2 req3 calc1 calc2 calc3
