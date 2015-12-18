/**
 *
 *client that calculates addition
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"data.h"
#define OP_FIFO_NAME "/tmp/my_co0"
#define RES_FIFO_NAME "/tmp/my_cr0"
int main()
{
	int rd_fd = 0,wr_fd = 0,res;
	DATA d;
	rd_fd = open(OP_FIFO_NAME,O_RDONLY);
	/*read operands and operators from server*/
	read(rd_fd,&d,sizeof(DATA));
	printf("add d.op1=%d,d.op2=%d,d.ops=%c\n",d.op1,d.op2,d.ops);
	res = d.op1+d.op2;
	printf("add result = %d\n",res);
	
	wr_fd = open(RES_FIFO_NAME,O_WRONLY);
	/*write the result to server*/
	write(wr_fd,&res,sizeof(int));
	
	close(rd_fd);
	close(wr_fd);

	return EXIT_SUCCESS;
}
