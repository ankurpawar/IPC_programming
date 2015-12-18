#include<stdio.h>
#include<stdlib.h>
#include"data.h"
#include<fcntl.h>

int main()
{
	int ret,fd_w,fd_r;
	DATA d;

	fd_w = open(FIFO_C_S,O_WRONLY);
	if (fd_w == -1) {
		perror("error in opening file");
		exit(EXIT_FAILURE);
	}

	fd_r = open(FIFO_S_C,O_RDONLY);
	if (fd_r == -1) {
		perror("error in opening file");
		exit(EXIT_FAILURE);
	}
	d.op1 = 1;
	d.op2 = 12;
	d.ops = '+';
	write(fd_w,&d,sizeof(DATA));
	read(fd_r,&ret,sizeof(int));
	printf("client d.op1=%d,d.op2=%d result = %d\n",d.op1,d.op2,ret);

	return EXIT_SUCCESS;
}
