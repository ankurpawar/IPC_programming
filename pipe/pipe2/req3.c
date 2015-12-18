/**
 *Client to request for multiplication 
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
        d.op1 = 3;
        d.op2 = 5;
        d.ops = '*';
        printf("mul argc=%d argv[0]=%s argv[1]=%s\n",argc,argv[0],argv[1]);
        
	wr_fd = atoi(argv[1]);
	rd_fd = atoi(argv[0]);
        
	write(wr_fd,&d,sizeof(DATA));
	read(rd_fd,&result,sizeof(int));
	printf("Final mul result = %d\n",result);
	
	return EXIT_SUCCESS;
}
