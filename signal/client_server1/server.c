#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include"data.h"

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
	int req_fd[2];
	struct sigaction act;
	DATA d;
	sigset_t mask,old_mask;
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
	
	ret_val = fork();
	if(ret_val == -1)
	{
		perror("error in fork");
		exit(EXIT_FAILURE);
	}
	/*parent block*/
	if(ret_val > 0)
	{
		printf("parent pid=%d\n",getpid());
		if( read(req_fd[0],&d,sizeof(DATA)) < 0)
		{
			perror("error in reading pipe\n");
			exit(EXIT_FAILURE);
		}
		printf("parent data d.op1=%d d.op2=%d d.ops=%c\n",d.op1,d.op2,d.ops);
		num = 19;
		write(req_fd[1],&num,sizeof(int));
		kill(ret_val,SIGUSR1);
	}
	else /*child block*/
	{
		printf("child pid=%d\n",getpid());
		DATA a;
		int  i = 0;
		a.op1 = 4; a.op2 = 9; a.ops = '+';
		if( write(req_fd[1],&a,sizeof(DATA)) < 0)
		{
			perror("error in writing pipe\n");
			exit(EXIT_FAILURE);
		}
		printf("\nchild data written \n");

		sigprocmask(SIG_BLOCK,&mask,&old_mask);
		while(!sig_flag)
			sigsuspend(&old_mask);
		sigprocmask(SIG_UNBLOCK,&mask,NULL);
		
		if( read(req_fd[0],&i,sizeof(int)) < 0)
		{
			perror("error in reading pipe\n");
			exit(EXIT_FAILURE);
		}
		printf("\nchild num=%d\n",i);
		exit(EXIT_SUCCESS);
	}

	ret_pid = wait(&ret_val);
	if (WIFEXITED(ret_val))
	{
        	printf("exited, status=%d\n", WEXITSTATUS(ret_val));
	}
	else if (WIFSIGNALED(ret_val)) 
	{
        	printf("killed by signal %d\n", WTERMSIG(ret_val));
        }

	printf("pid = %d,val=%d",ret_pid,ret_val);
	printf("all done \n");
	return EXIT_SUCCESS;
}
