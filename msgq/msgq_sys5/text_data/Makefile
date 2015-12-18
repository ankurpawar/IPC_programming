all : process1 process2

process1 : process1.c
	gcc -o $@ $^ 

process2 : process2.c
	gcc -o $@ $^

clean:
	-rm -f process1 process2
