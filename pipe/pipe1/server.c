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

int main() 
{
	int ret_pid;
	int parent_pid;
	int ret_val;
	int req_fds[2];
	int calc_fds[2];
	int i;
	char rd_str1[8],wr_str1[8];
	char rd_str2[8],wr_str2[8];
	char *req_path[] = {"./req1","./req2","./req3"};
	char *calc_path[] = {"./calc1","./calc2","./calc3"};
	DATA d[3];
	int result_dat = 0;
	/*Create Pipe*/
	pipe(req_fds);
	pipe(calc_fds);
	/*creating req client*/
	for(i = 0;i < 3; i++)
	{
		ret_pid = fork();
		
		if(ret_pid > 0)
		{
			printf("parent block pid = %d\n",getpid());
			read(req_fds[0],&d[i],sizeof(DATA));
			printf("op1 = %d, op2 = %d,ops = %c\n",d[i].op1,d[i].op2,d[i].ops);
			//write()
		}
		else
		{
			sprintf(rd_str1,"%d",req_fds[0]);
			sprintf(wr_str1,"%d",req_fds[1]);
			printf("child req block pid = %d\n",getpid());
			execl(req_path[i],rd_str1,wr_str1,NULL);	
			break;
		}

	}
	
	for(i = 0 ; i < 3; i++)
	{
		ret_pid = fork();
		if(ret_pid > 0)
		{
			printf("parent block pid = %d\n",getpid());
			write(calc_fds[1],&d[i],sizeof(DATA));
			printf("op1 = %d, op2 = %d,ops = %c\n",d[i].op1,d[i].op2,d[i].ops);
			sleep(1);
			read(calc_fds[0],&result_dat,sizeof(int));
			printf("result = %d\n",result_dat);
			write(req_fds[1],&result_dat,sizeof(int));
		}
		else
		{
			sprintf(rd_str2,"%d",calc_fds[0]);
			sprintf(wr_str2,"%d",calc_fds[1]);
			printf("child req block pid = %d\n",getpid());
			switch(d[i].ops)
			{
				case '+':
					execl(calc_path[0],rd_str2,wr_str2,NULL);	
					break;
				case '-':
					execl(calc_path[1],rd_str2,wr_str2,NULL);	
					break;
				case '*':
					execl(calc_path[2],rd_str2,wr_str2,NULL);	
					break;
			}
			break;
		}
	}	

	for(i = 0 ;i < 6; i++)
	{
		ret_pid = wait(&ret_val);
		printf("pid = %d , status = %d \n",ret_pid,ret_val);
	}
	return EXIT_SUCCESS;
}
