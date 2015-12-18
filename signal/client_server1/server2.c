/**
 *Program to demonstrate use of pipes and signals
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include"data.h"

#define NUM_REQ_CLIENT 3
#define NUM_PROC_CLIENT 3
 
struct PID_OPS
{
	int pid;
	char ops;
};

int sig_flag = 0;

void sig_usr_handler(int signal)
{
	sig_flag = 1;
}

int main()
{
	int ret_pid = 0;
	int ret_val = 0;
	int num = 0 ;
	int count = 0;
	int req_fd[2];
	int proc_fd[2];
	struct sigaction act;
	DATA d[3] = {{1,4,'+'},{6,9,'-'},{18,2,'*'}};
	DATA a[3];
	int pid[3];
	int i = 0;
	sigset_t mask,old_mask;
	struct PID_OPS pid_ops[3];

	act.sa_handler = sig_usr_handler;
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR1);

	act.sa_handler = sig_usr_handler;
	sigemptyset(&mask);
	sigaddset(&mask,SIGUSR1);
	sigaction(SIGUSR1,&act,0);

	if( pipe(req_fd) < 0 )
	{
		perror("error in creating pipe\n");
		exit(EXIT_FAILURE);
	}

	if( pipe(proc_fd) < 0)
	{	
		perror("error in creating pipe\n");
		exit(EXIT_FAILURE);
	}

	i = 0;
	while(i < NUM_REQ_CLIENT)
	{	
		ret_val = fork();
		if(ret_val == -1)
		{
			perror("error in fork");
			exit(EXIT_FAILURE);
		}
		pid[i] = ret_val;
		/*parent block*/
		if(ret_val > 0)
		{
			printf("parent pid=%d\n",getpid());
			if( read(req_fd[0],&a[i],sizeof(DATA)) < 0)
			{
				perror("error in reading pipe\n");
				exit(EXIT_FAILURE);
			}
			pid_ops[i].pid = ret_val;
			pid_ops[i].ops = d[i].ops;
			printf("parent data d.op1=%d d.op2=%d d.ops=%c\n",d[i].op1,d[i].op2,d[i].ops);
		}	
		else /*child block*/
		{
			printf("child pid=%d\n",getpid());
			int  count = 0;
			if( write(req_fd[1],&d[i],sizeof(DATA)) < 0)
			{
				perror("error in writing pipe\n");
				exit(EXIT_FAILURE);
			}
			printf("\nchild data written \n");

			sigprocmask(SIG_BLOCK,&mask,&old_mask);
			while(!sig_flag)
				sigsuspend(&old_mask);
			sigprocmask(SIG_UNBLOCK,&mask,NULL);
			sig_flag = 0;
			if( read(req_fd[0],&i,sizeof(int)) < 0)
			{
				perror("error in reading pipe\n");
				exit(EXIT_FAILURE);
			}
			printf("\nchild num=%d\n",i);
			exit(EXIT_SUCCESS);
			/*requesting child finished*/
		}
		i++;
	}

	i = 0;
	/*create processing client*/
	while(i < NUM_PROC_CLIENT)
	{
		ret_val = fork();
		pid[i] = ret_val;
		if(ret_val == -1)
		{
			perror("error in fork\n");
			exit(EXIT_FAILURE);	
		}
		/*parent*/
		if(ret_val > 0)
		{
			int count = 0;
			int j = 0;
			int wake_pid = 0;
			write(proc_fd[1],&a[i],sizeof(DATA));
			/*wait for processing client to write data*/
			sigprocmask(SIG_BLOCK,&mask,&old_mask);
			while(!sig_flag)
				sigsuspend(&old_mask);
			sigprocmask(SIG_UNBLOCK,&mask,NULL);
			sig_flag = 0;	

			read(proc_fd[0],&count,sizeof(int));
			printf("res = %d\n",count);
			write(req_fd[1],&count,sizeof(int));
			while(j < 3)
			{
				if(pid_ops[j].ops == a[i].ops)
					wake_pid = pid_ops[j].pid;
				j++;
			}
			printf("wake_pid = %d\n",wake_pid);
			kill(wake_pid,SIGUSR1);
		}
		else /*child*/
		{
			DATA b;
			int res = 0;
			read(proc_fd[0],&b,sizeof(DATA));
			printf("b.op1=%d b.op2=%d b.ops=%c\n",b.op1,b.op2,b.ops);
			switch(b.ops)
			{
				case '+': 
					res = b.op1 + b.op2;
					break;
				case '-':
					res = b.op1 - b.op2;
					break;
				case '*':
					res = b.op1 * b.op2;
					break;
			}
			write(proc_fd[1],&res,sizeof(int));
			kill(getppid(),SIGUSR1);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
	/*waiting for all the child to be terminated*/
	i = 0;
	while(i < (NUM_REQ_CLIENT+NUM_PROC_CLIENT))
	{
		ret_pid = wait(&ret_val);
		if (WIFEXITED(ret_val))
		{
        		printf("exited, status=%d\n", WEXITSTATUS(ret_val));
		}
		else if (WIFSIGNALED(ret_val)) 
		{
        		printf("killed by signal %d\n", WTERMSIG(ret_val));
        	}
		i++;
		printf("pid = %d,val=%d",ret_pid,ret_val);
	}
	printf("all done \n");
	return EXIT_SUCCESS;
}
