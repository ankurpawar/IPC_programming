/**
 *Client that calculate subtraction 
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
        printf("argc=%d\n",argc);
        printf("argv[0]=%s\n",argv[0]);
        printf("argv[1]=%s\n",argv[1]);
        rd_fd = atoi(argv[0]);
        wr_fd = atoi(argv[1]);
        read(rd_fd,&d,sizeof(DATA));
        printf("op1 = %d,op2 = %d,ops=%c\n",d.op1,d.op2,d.ops);
        result = d.op1 - d.op2;
        write(wr_fd,&result,sizeof(int));
	printf("process subtraction child1\n");
	return EXIT_SUCCESS;
}
