/**
 *Program to demonstrate Server client communication using
 *pipes  
 *
 *
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include"data.h"
#include<unistd.h>
#include<fcntl.h>

#define NUM_CHILD 1

int main() 
{
	int ret_pid[NUM_CHILD];
	int pid;
	int ret_val;
	int req_fds[2];
	int calc_fds[2];
	int i;
	DATA d[3];
	int result_dat = 0;
	/*Create Pipe*/
	pipe(req_fds);
	pipe(calc_fds);
	/*creating req client*/
	for(i = 0;i < NUM_CHILD; i++)
	{
		ret_pid[i] = fork();
		
		if(ret_pid[i] > 0)
		{
			printf("parent block1 pid = %d\n",getpid());
			/*read data from requesting client*/
			read(req_fds[0],&d[i],sizeof(DATA));
			printf("op1=%d , op2=%d , ops=%c \n",d[i].op1,d[i].op2,d[i].ops);
			write(calc_fds[1],&d[i],sizeof(DATA));
		}
		else
		{
			DATA a; 		
			a.op1 = 1;a.op2 = 2;a.ops = '+';
			write(req_fds[1],&a,sizeof(DATA));
			printf("child block1 pid = %d\n",getpid());
			break;
		}

	}
	
	for(i = 0 ; i < NUM_CHILD; i++)
	{
		pid = 0;
		if(ret_pid[i] > 0)
			pid = fork();
		if(pid > 0)
		{
			int res;
			printf("parent block2 pid = %d\n",getpid());
			/*write data to calculating client*/
			read(calc_fds[0],&res,sizeof(int));
			printf("result = %d\n",res);

			/*wait for child to be terminated*/
			for(i = 0 ;i < 2*NUM_CHILD; i++)
			{
				pid = wait(&ret_val);
				printf("pid = %d , status = %d \n",pid,ret_val);
			}
			printf("parent:all done\n");
		}
		else
		{
			DATA b;
			int res;
			printf("child block2 pid = %d\n",getpid());
			/*read operands calculate and write result*/
			read(calc_fds[0],&b,sizeof(DATA));
			printf("calc op1=%d , op2=%d , ops=%c \n",b.op1,b.op2,b.ops);
			res = b.op1 + b.op2;
			write(calc_fds[1],&res,sizeof(int));
			break;
		}
	}
	return EXIT_SUCCESS;
}
