#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<semaphore.h>
#include<pthread.h>
#include<errno.h>
#include<signal.h>
#include<unistd.h>
#include"data.h"

char sig_flag1 = 0;
char sig_flag2 = 0;

void sig_usr1_handler(int signal)
{
        sig_flag1 = 1;
}

void sig_usr2_handler(int signal)
{
        sig_flag2 = 1;
}



int main()
{
	int ret,fd_w,fd_r;
	DATA d;
	sem_t *sem1;
	sem_t *sem2;
	sem_t *sem3;
	struct sigaction act1;
	struct sigaction act2;
	sigset_t mask1,old_mask1;
	sigset_t mask2,old_mask2;

        act1.sa_handler = sig_usr1_handler;
        sigemptyset(&mask1);
        sigaddset(&mask1,SIGUSR1);
        sigaction(SIGUSR1,&act1,0);

        act2.sa_handler = sig_usr2_handler;
        sigemptyset(&mask2);
        sigaddset(&mask2,SIGUSR2);
        sigaction(SIGUSR2,&act2,0);


	sem1  = sem_open(SEM_NAME1,O_CREAT,0666,1);
	if (sem1 == SEM_FAILED) {
		perror("Error in creating semaphore:");
		exit(EXIT_FAILURE);
	}

	sem2  = sem_open(SEM_NAME2,O_CREAT,0666,1);
	if (sem2 == SEM_FAILED) {
		perror("Error in creating semaphore:");
		exit(EXIT_FAILURE);
	}
	
	sem3  = sem_open(SEM_NAME3,O_CREAT,0666,0);
	if (sem3 == SEM_FAILED) {
		perror("Error in creating semaphore:");
		exit(EXIT_FAILURE);
	}
	
	d.pid = getpid();
	d.op1 = 6;
	d.op2 = 9;
	d.ops = '-';
	printf("client2 sem_wait write\n");
	
	sem_wait(sem1);
	fd_w = open(FIFO_C_S,O_WRONLY);
	if (fd_w == -1) {
		perror("error in opening file");
		exit(EXIT_FAILURE);
	}
	ret = write(fd_w,&d.pid,sizeof(int));
	printf("%d ",ret);
	ret = write(fd_w,&d.op1,sizeof(int));
	printf("%d ",ret);
	ret = write(fd_w,&d.op2,sizeof(int));
	printf("%d ",ret);
	ret = write(fd_w,&d.ops,sizeof(char));
	printf("%d \n",ret);
	/*sem_wait(sem3);*/
	printf("sem3 wait done\n");
	sem_post(sem1);

	fd_r = open(FIFO_S_C,O_RDONLY);
	if (fd_r == -1) {
		perror("error in opening file");
		exit(EXIT_FAILURE);
	}
	
	sigprocmask(SIG_BLOCK,&mask1,&old_mask1);
	while(!sig_flag1)
		sigsuspend(&old_mask1);
	sigprocmask(SIG_UNBLOCK,&mask1,NULL);
	sig_flag1 = 0;
	
	printf("suspend done\n");
	read(fd_r,&ret,sizeof(int));
	close(fd_r);
	printf("client pid=%d d.op1=%d,d.op2=%d result = %d\n",d.pid,d.op1,d.op2,ret);
	
	sigprocmask(SIG_BLOCK,&mask2,&old_mask2);
	while(!sig_flag2)
		sigsuspend(&old_mask2);
	sigprocmask(SIG_UNBLOCK,&mask2,NULL);
	sig_flag2 = 0;
	
	close(fd_w);
	ret = sem_close(sem1);
	if (ret == -1) {
		perror("error in closing semaphore1:");
		exit(EXIT_FAILURE);
	}
	
	ret = sem_close(sem2);
	if (ret == -1) {
		perror("error in closing semaphore2:");
		exit(EXIT_FAILURE);
	}
	
	ret = sem_close(sem3);
	if (ret == -1) {
		perror("error in closing semaphore3:");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}
