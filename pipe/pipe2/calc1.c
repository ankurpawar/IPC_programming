/**
 *Client that calculate addition 
 *
 */

#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include"data.h"

int main(int argc,char *argv[])
{
	DATA d;
	int wr_fd = 0;
	int rd_fd = 0;
	int result = 0;
	printf("add argc=%d argv[0]=%s argv[1]=%s\n",argc,argv[0],argv[1]);
	rd_fd = atoi(argv[0]);
	wr_fd = atoi(argv[1]);
	read(rd_fd,&d,sizeof(DATA));
	printf("add op1 = %d,op2 = %d,ops=%c\n",d.op1,d.op2,d.ops);
	result = d.op1 + d.op2;
	write(wr_fd,&result,sizeof(int));
	return EXIT_SUCCESS;
}
