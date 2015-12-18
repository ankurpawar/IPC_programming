/**
 *
 *Requesting client for addition
 */
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include"data.h"

#define OP_FIFO_NAME "/tmp/my_ro0"
#define RES_FIFO_NAME "/tmp/my_rr0"

int main()
{
	int res,rd_fd,wr_fd ;
	DATA d;
	d.op1 = 6;
	d.op2 = 8;
	d.ops = '+';
	/*write operands and operator to sever*/
	wr_fd = open(OP_FIFO_NAME,O_WRONLY);
	write(wr_fd,&d,sizeof(DATA));
	printf("add request\n");

	/*read result from server*/       
	rd_fd = open(RES_FIFO_NAME,O_RDONLY);
	read(rd_fd,&res,sizeof(int));
	
	close(rd_fd);
	close(wr_fd);
	return EXIT_SUCCESS;
}
