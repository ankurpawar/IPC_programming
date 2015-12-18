/**
 *Client to request for addition 
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
	d.op1 = 13;
	d.op2 = 9;
	d.ops = '+';
	printf("argc=%d\n",argc);
	printf("argv[0]=%s\n",argv[0]);
	printf("argv[1]=%s\n",argv[1]);
	wr_fd = atoi(argv[1]);
	write(wr_fd,&d,sizeof(DATA));
	printf("request addition child1\n");
	sleep(3);
	rd_fd = atoi(argv[0]);
	read(rd_fd,&result,sizeof(int));
	printf("Final add result = %d \n",result);
	return EXIT_SUCCESS;
}
