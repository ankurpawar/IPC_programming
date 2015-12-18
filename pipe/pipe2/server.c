/**
 *Program to demonstrate Server client communication using
 *multiple pipes  
 *
 *
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include"data.h"
#include<unistd.h>
#include<fcntl.h>

typedef struct pid_op
{
	int pid;
	int req_rd_fd;
	int req_wr_fd;
	int calc_rd_fd;
	int calc_wr_fd;
	DATA d;
}PID_OP;

int main() 
{
	int ret_pid;
	int parent_pid;
	int ret_val;
	int req_res_fds[3][2];
	int req_op_fds[3][2];
	int calc_res_fds[3][2];
	int calc_op_fds[3][2];
	int i;
	char rd_str1[8],wr_str1[8];
	char rd_str2[8],wr_str2[8];

	char *req_path[] = {"./req1","./req2","./req3"};
	char *calc_path_add = "./calc1";
	char *calc_path_sub = "./calc2";
	char *calc_path_mul = "./calc3";

	DATA d[3];
	int result_dat = 0;
	PID_OP pid_op_table[3];

	/*Create Pipe*/
	pipe(req_op_fds[0]);
	pipe(req_op_fds[1]);
	pipe(req_op_fds[2]);

	pipe(req_res_fds[0]);
	pipe(req_res_fds[1]);
	pipe(req_res_fds[2]);

	pipe(calc_op_fds[1]);
	pipe(calc_op_fds[2]);
	pipe(calc_op_fds[0]);

	pipe(calc_res_fds[0]);
	pipe(calc_res_fds[1]);
	pipe(calc_res_fds[2]);
	/*creating req client*/
	for(i = 0;i < 3; i++)
	{
		ret_pid = fork();
		
		if(ret_pid > 0)
		{
			printf("parent block pid = %d\n",getpid());
			read(req_op_fds[i][0],&d[i],sizeof(DATA));
			printf("op1 = %d, op2 = %d,ops = %c\n",d[i].op1,d[i].op2,d[i].ops);
			pid_op_table[i].pid = ret_pid;
			pid_op_table[i].req_rd_fd = req_op_fds[i][0];
			pid_op_table[i].req_wr_fd = req_res_fds[i][1];
			pid_op_table[i].d.op1 = d[i].op1;
			pid_op_table[i].d.op2 = d[i].op2;
			pid_op_table[i].d.ops = d[i].ops;
			pid_op_table[i].calc_wr_fd = calc_op_fds[i][1];
			pid_op_table[i].calc_rd_fd = calc_res_fds[i][0];
		}
		else
		{
			sprintf(rd_str1,"%d",req_res_fds[i][0]);
			sprintf(wr_str1,"%d",req_op_fds[i][1]);
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
			printf("write on calc parent block pid = %d\n",getpid());
			write(pid_op_table[i].calc_wr_fd,&(pid_op_table[i].d),sizeof(DATA));
			//printf("op1 = %d, op2 = %d,ops = %c\n",pid_op_table[i].d.op1,pid_op_table[i].d.op2,pid_op_table[i].d.ops);
			read(pid_op_table[i].calc_rd_fd,&result_dat,sizeof(int));
			printf("result = %d\n",result_dat);
			write(pid_op_table[i].req_wr_fd,&result_dat,sizeof(int));
		}
		else
		{
			sprintf(rd_str2,"%d",calc_op_fds[i][0]);
			sprintf(wr_str2,"%d",calc_res_fds[i][1]);
			switch(pid_op_table[i].d.ops)
			{
				case '+':
					execl(calc_path_add,rd_str2,wr_str2,NULL);
					break;
				case '-':
					execl(calc_path_sub,rd_str2,wr_str2,NULL);
					break;
				case '*':
					execl(calc_path_mul,rd_str2,wr_str2,NULL);
					break;
			}
			break;
		}
	}	

	for(i = 0 ;i < 3; i++)
	{
		ret_pid = wait(&ret_val);
		//printf("pid = %d , status = %d \n",ret_pid,ret_val);
	}
	return EXIT_SUCCESS;
}
