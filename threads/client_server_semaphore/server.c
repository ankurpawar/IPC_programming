/*
 *Program to demonstrate multithreading using server client
 *
 *Server : reads a struct that contains pid,operands,operator,evaluates 
 *result and writes back to client
 *
 *Client : writes a struct that contains its own pid,operands and operator
 *reads back te result
 *
 *Server creates 5 threads and runs them concurrently.Each thread signal 
 *client to read the result.One big problem that came during the development
 *was closing of fifo.Any client closing fifo result immediate return from
 *read in server sor it doesnot affect the thread that closes it because
 *its data is received.But it affect that client whos data server is reading.
 *e.g. server is reading data from client1 and client2 closes the fifo.In this
 *situation read returns 0.Semaphore give atomic access to data but signals
 *are used here for signaling client to close the fifo.Write by the client
 *and the server are both protected by semaphore.
 *
 *semaphore value could be greater than one.if server or client is terminated
 *the value will persist and on the next sem_open the value remained will be
 *considered not the one provided in sem_open
 *
 *Start server first it, takes care of initial value of semaphore. 
 *If semaphore already exists then it has some value.The value should be
 *adjusted first for proper sync of both server and client.
 *If client runs first it intialize semaphore and server will adjust value
 *this will make server functioning improper.
 *
 *better solution is to use mutex.
 *
 *other solution is to use message q .it doesnt even need mutex
 */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<semaphore.h>
#include<signal.h>
#include<errno.h>
#include"data.h"

#define MAX_THREAD 5


void *process_add(void *arg);
void *process_sub(void *arg);
void *process_mul(void *arg);

void create_fifo(char *fifo_name,int mode);

int fd_w;
int fd_r;
sem_t *sem1;
sem_t *sem2;
sem_t *sem3;

int main()
{
	int ret,i,j,thread_count;
	void *ret_val;
	DATA d1[MAX_THREAD];
	pthread_t thread_arr[MAX_THREAD];
	sem1 = sem_open(SEM_NAME1,O_CREAT,0666,1);
	if (sem1 == SEM_FAILED) {
		perror("error in creating semaphore");
		exit(EXIT_FAILURE);
	}

	sem2 = sem_open(SEM_NAME2,O_CREAT,0666,1);
	if (sem2 == SEM_FAILED) {
		perror("error in creating semaphore");
		exit(EXIT_FAILURE);
	}
	
	sem3 = sem_open(SEM_NAME3,O_CREAT,0666,0);
	if (sem3 == SEM_FAILED) {
		perror("error in creating semaphore");
		exit(EXIT_FAILURE);
	}
	
	ret = sem_getvalue(sem1,&i);
	if (ret == -1) {
		perror("error in getting value:");
		exit(EXIT_FAILURE);
	}
	printf("sem1 value = %d\n",i);
	if (i == 0)
		sem_post(sem1);
	else if (i == 2)
		sem_wait(sem1);

	ret = sem_getvalue(sem2,&i);
	if (ret == -1) {
		perror("error in getting value:");
		exit(EXIT_FAILURE);
	}
	printf("sem2 value = %d\n",i);
	if (i == 0)
		sem_post(sem2);
	else if (i == 2)
		sem_wait(sem2);

	ret = sem_getvalue(sem3,&i);
	if (ret == -1) {
		perror("error in getting value:");
		exit(EXIT_FAILURE);
	}
	printf("sem3 value = %d\n",i);
	if (i == 1)
		sem_wait(sem3);
	else if (i == 2)
		sem_wait(sem3);
	/*make fifo for reading and writing data*/
	/*client will write structire on this fifo.*/
	create_fifo(FIFO_C_S,O_RDONLY);
	/*server write result on this fifo.The result is written 
	  by threads so semaphore is used for sync access.*/	
	create_fifo(FIFO_S_C,O_WRONLY);

		
	for(j = 0;j < 1;j++)
	{
		for (i = 0,thread_count = 0;i < MAX_THREAD;i++) {
			fd_r = open(FIFO_C_S,O_RDONLY);
			if (fd_r == -1) {
				perror("error in opening file");
				exit(EXIT_FAILURE);
			}
			printf("server_read ");
			ret = read(fd_r,&d1[i].pid,sizeof(int));
			printf("%d ",ret);
			ret = read(fd_r,&d1[i].op1,sizeof(int));
			printf("%d ",ret);
			ret = read(fd_r,&d1[i].op2,sizeof(int));
			printf("%d ",ret);
			ret = read(fd_r,&d1[i].ops,sizeof(char));
			printf("%d \n",ret);
			printf("sem3 post\n");	
			/*sem_post(sem3);*/
			close(fd_r);
			printf("pid=%d d.op1=%d d.op2=%d d1.ops=%c i=%d\n",d1[i].pid,d1[i].op1,d1[i].op2,d1[i].ops,i);
			switch (d1[i].ops) {
			case '+':
				pthread_create(&thread_arr[i],NULL,process_add,&d1[i]);
				thread_count++;
			break ;
			case '-':
				pthread_create(&thread_arr[i],NULL,process_sub,&d1[i]);
				thread_count++;
			break ;
			case '*':
				pthread_create(&thread_arr[i],NULL,process_mul,&d1[i]);
				thread_count++;
			break ;
			default:
			break;
			}
		}
		for (i = 0;i < thread_count;i++) {
			printf("joining %d \n",i);
			ret = pthread_join(thread_arr[i],&ret_val);
			if (ret != 0) {
				printf("error = %d\n",ret);
				perror("error in joining thread");
			}
			kill(d1[i].pid,SIGUSR2);
			free(ret_val);
		}	
	}
	/**/
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
	
	ret = sem_unlink(SEM_NAME1);
	if (ret == -1) {
		perror("error in unlink semaphore1:");
		exit(EXIT_FAILURE);
	}
	
	ret = sem_unlink(SEM_NAME2);
	if (ret == -1) {
		perror("error in unlink semaphore2:");
		exit(EXIT_FAILURE);
	}
	
	ret = sem_unlink(SEM_NAME3);
	if (ret == -1) {
		perror("error in unlink semaphore3:");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}

void create_fifo(char *fifo_name,int mode)
{
	int ret;
	ret = access(fifo_name,F_OK);
	printf("ret == %d\n",ret); 
	if (ret == -1) {
		ret = mkfifo(fifo_name,0666);
		if (ret == -1) {
			perror("error in creating fifo");
			exit(EXIT_FAILURE);
		}
	}

}

void *process_add(void *arg)
{
	DATA d;
	int *result;
	memcpy((void*)&d,arg,sizeof(DATA));
	result = (int *)malloc(sizeof(int));
	*result = d.op1 + d.op2;
	printf("process pid =%d op1 = %d,op2 = %d,ops = %c result = %d \n",d.pid,d.op1,d.op2,d.ops,*result);
	/*signal each client with pid here*/
	sem_wait(sem2);
	fd_w = open(FIFO_S_C,O_WRONLY);
	printf("open done\n");
	if (fd_w == -1) {
		perror("error in opening file");
		exit(EXIT_FAILURE);
	}
	write(fd_w,result,sizeof(int));
	close(fd_w);	
	kill(d.pid,SIGUSR1);
	sem_post(sem2);
	pthread_exit(result);
}

void *process_sub(void *arg)
{
	DATA d;
	int *result;
	memcpy((void*)&d,arg,sizeof(DATA));
	result = (int *)malloc(sizeof(int));
	*result = d.op1 - d.op2;
	/*printf("process pid =%d op1 = %d,op2 = %d,ops = %c result = %d \n",d.pid,d.op1,d.op2,d.ops,*result);*/
	/*signal each client with pid here*/
	sem_wait(sem2);
	fd_w = open(FIFO_S_C,O_WRONLY);
	printf("open done\n");
	if (fd_w == -1) {
		perror("error in opening file");
		exit(EXIT_FAILURE);
	}
	write(fd_w,result,sizeof(int));
	close(fd_w);	
	kill(d.pid,SIGUSR1);
	sem_post(sem2);
	pthread_exit(result);
}

void *process_mul(void *arg)
{
	DATA d;
	int *result;
	memcpy((void*)&d,arg,sizeof(DATA));
	result = (int *)malloc(sizeof(int));
	*result = d.op1 * d.op2;
	/*printf("process pid =%d op1 = %d,op2 = %d,ops = %c result = %d \n",d.pid,d.op1,d.op2,d.ops,*result);*/
	/*signal each client with pid here*/
	sem_wait(sem2);
	fd_w = open(FIFO_S_C,O_WRONLY);
	printf("open done\n");
	if (fd_w == -1) {
		perror("error in opening file");
		exit(EXIT_FAILURE);
	}
	write(fd_w,result,sizeof(int));
	close(fd_w);	
	kill(d.pid,SIGUSR1);
	sem_post(sem2);
	pthread_exit(result);
}
