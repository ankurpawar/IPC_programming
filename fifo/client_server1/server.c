/**
 *
 *program to demostrate the FIFO for client server communication
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include"data.h"
#include<sys/types.h>
#include<sys/stat.h>

#define FIFO_REQ_OP "/tmp/my_ro%d"
#define FIFO_REQ_RES "/tmp/my_rr%d"
#define FIFO_CALC_OP "/tmp/my_co%d"
#define FIFO_CALC_RES "/tmp/my_cr%d"
#define NUM_OPS 3

void create_fifo(char *name,int i);

int main()
{
	int i,res;
	
	/*3 requesting clients and 3 calculating cleints
	  each requesting client need 2 fifo 1 to write request
	  and another one to read result from server.
	  In the same way calculating client requires 2 fifo
	  one to read operands from server and another one to 
	  write result.Total 12 fifos
	*/
	int fifo_req_op_fd[3] = {0};
	int fifo_req_res_fd[3] = {0};
	int fifo_calc_op_fd[3] = {0};
	int fifo_calc_res_fd[3] = {0};

	/*buffer to be used with sprintf*/
	char fifo_name[32] = {0};
	char fifo_name2[32] = {0}; 

	DATA d[NUM_OPS];
	
	for(i = 0; i < NUM_OPS ; i++)
	{	
		sprintf(fifo_name,FIFO_REQ_OP,i);
		printf("FIFO_REQ_OP name = %s \n",fifo_name);
		create_fifo(fifo_name,i);
		fifo_req_op_fd[i] = open(fifo_name,O_RDONLY);
		/*read the operand and operator from requesting client*/
		read(fifo_req_op_fd[i],&d[i],sizeof(DATA));
		printf("d.op1=%d,d.op2=%d,d.ops=%c\n",d[i].op1,d[i].op2,d[i].ops);
		close(fifo_req_op_fd[i]);
	}

	for(i = 0; i < NUM_OPS ; i++)
	{
		sprintf(fifo_name,FIFO_CALC_OP,i);
		create_fifo(fifo_name,i);
		printf("FIFO_CALC_OP name = %s \n",fifo_name);
		fifo_calc_op_fd[i] = open(fifo_name,O_WRONLY);
		/*write the operand and operator to calc client*/
		write(fifo_calc_op_fd[i],&d[i],sizeof(DATA));
		

		sprintf(fifo_name,FIFO_CALC_RES,i);
		create_fifo(fifo_name,i);
		printf("FIFO_CALC_RES name = %s \n",fifo_name);
		fifo_calc_res_fd[i] = open(fifo_name,O_RDONLY);
		/*read the result from calc client*/
		read(fifo_calc_res_fd[i],&res,sizeof(int));
		printf("result = %d \n",res);

		
		sprintf(fifo_name,FIFO_REQ_RES,i);
		create_fifo(fifo_name,i);
		printf("FIFO_REQ_RES name = %s \n",fifo_name);
		fifo_req_res_fd[i] = open(fifo_name,O_WRONLY);
		/*write back the result to requesting client*/
		write(fifo_req_res_fd[i],&res,sizeof(int));
		/*data written*/
		close(fifo_calc_op_fd[i]);
		close(fifo_req_res_fd[i]);
		close(fifo_calc_res_fd[i]);
	}

	
	printf("all done\n");
	return 	EXIT_SUCCESS;
}

void create_fifo(char *fifo_name,int i)
{
	int ret = 0;
	if(access(fifo_name,F_OK)==-1)
	{
		ret = mkfifo(fifo_name,0777);
		if(ret < 0)
		{
			fprintf(stderr,"Error in creating fifo %s",fifo_name);
			exit(EXIT_FAILURE);
		}
	}

}
