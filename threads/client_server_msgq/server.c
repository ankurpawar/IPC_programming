/*
 *Program to demonstrate multithreading using server client
 *
 *Client : sends its pid along with operator1 , operator2 and operand in a message queue.In message
 *         type field is kept 1.To identify client message.
 *Server : evaluates result and send it in a message queue.Server writes pid of child in type field
 *         so that the result is received by intended client.
 *Message queue eliminates the need of semaphore,mutex and signals.
 *First server allocates memory and copy the received data and pass the address of memory to
 *thread.Doing this makes separate copy of data for each thread.When thread join this memory is released.
 *Server cant remove message queue after joining all thread because some client have not retrieved
 *their result from queue.Before removing queue ensure that is there any client left to retrieve
 *its result.Then remove the queue.
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<sys/ipc.h>
#include"data.h"

#define MAX_THREAD 5


void *process_add(void *arg);
void *process_mul(void *arg);
void *process_sub(void *arg);

int msgq_cs;
int msgq_sc;

int main()
{
	int ret,i,j,thread_count;
	void *ret_val;
	MSG_CLIENT data_rcv;
	int pid_arr[MAX_THREAD];
	pthread_t thread_arr[MAX_THREAD];
	MSG_CLIENT *client_data[MAX_THREAD];
	
	/*make message q for reading and writing data*/
	msgq_cs = msgget(MSGQ_C_S,0666|IPC_CREAT);
	if (msgq_cs == -1) {
		perror("error in creating msg Q CS:");
		exit(EXIT_FAILURE);
	}
	
	msgq_sc = msgget(MSGQ_S_C,0666|IPC_CREAT);
	if (msgq_sc == -1) {
		perror("error in creating msg Q SC:");
		exit(EXIT_FAILURE);
	}
		
	for(j = 0;j < 1;j++)
	{
		for (i = 0,thread_count = 0;i < MAX_THREAD;i++) {
			ret = msgrcv(msgq_cs,&data_rcv,sizeof(MSG_CLIENT),1,0);
			if (ret == -1) {
				perror("error in reading msg Q CS");
				exit(EXIT_FAILURE);
			}
			printf("pid=%d d.op1=%d d.op2=%d d1.ops=%c i=%d\n",data_rcv.pid,data_rcv.op1,data_rcv.op2,data_rcv.ops,i);
			
			client_data[i] = (MSG_CLIENT *) malloc(sizeof(MSG_CLIENT));
			if (client_data[i] == NULL) {
				perror("error in malloc:");
				exit(EXIT_FAILURE);
			}
			memcpy(client_data[i],&data_rcv,sizeof(MSG_CLIENT));
			
			switch (data_rcv.ops) {
			case '+':
				pthread_create(&thread_arr[i],NULL,process_add,client_data[i]);
				thread_count++;
			break ;
			case '-':
				pthread_create(&thread_arr[i],NULL,process_sub,client_data[i]);
				thread_count++;
			break ;
			case '*':
				pthread_create(&thread_arr[i],NULL,process_mul,client_data[i]);
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
			free(ret_val);
			free(client_data[i]);	
		}	
	}
	
	/*ret = msgctl(msgq_cs, IPC_RMID, NULL);
	if(ret == -1) {
		perror("error in removing Q:");
		exit(EXIT_FAILURE);
	}
	
	ret = msgctl(msgq_sc, IPC_RMID, NULL);
	if(ret == -1) {
		perror("error in removing Q:");
		exit(EXIT_FAILURE);
	}*/
		
	return EXIT_SUCCESS;
}

void *process_add(void *arg)
{
	MSG_SERVER data_snd;
	MSG_CLIENT *data_rcv;
	int *result;
	int ret;

	data_rcv = (MSG_CLIENT *)arg;
	result = (int *)malloc(sizeof(int));
	*result = data_rcv->op1 + data_rcv->op2;
	printf("process pid =%d op1 = %d,op2 = %d,ops = %c result = %d \n",data_rcv->pid,
						data_rcv->op1,data_rcv->op2,data_rcv->ops,*result);
	data_snd.type = data_rcv->pid;
	data_snd.result = *result;
	
	ret = msgsnd(msgq_sc,&data_snd,sizeof(int),data_rcv->pid);
	if (ret == -1) {
		perror("error in sending data:");
		exit(EXIT_FAILURE);
	}
	pthread_exit(result);
}


void *process_sub(void *arg)
{
	MSG_SERVER data_snd;
	MSG_CLIENT *data_rcv;
	int *result;
	int ret;

	data_rcv = (MSG_CLIENT *)arg;
	result = (int *)malloc(sizeof(int));
	*result = data_rcv->op1 - data_rcv->op2;
	printf("process pid =%d op1 = %d,op2 = %d,ops = %c result = %d \n",data_rcv->pid,
						data_rcv->op1,data_rcv->op2,data_rcv->ops,*result);
	data_snd.type = data_rcv->pid;
	data_snd.result = *result;
	
	ret = msgsnd(msgq_sc,&data_snd,sizeof(int),data_rcv->pid);
	if (ret == -1) {
		perror("error in sending data:");
		exit(EXIT_FAILURE);
	}
	pthread_exit(result);
}


void *process_mul(void *arg)
{
	MSG_SERVER data_snd;
	MSG_CLIENT *data_rcv;
	int *result;
	int ret;
	
	data_rcv = (MSG_CLIENT *)arg;
	result = (int *)malloc(sizeof(int));
	*result = data_rcv->op1 * data_rcv->op2;
	printf("process pid =%d op1 = %d,op2 = %d,ops = %c result = %d \n",data_rcv->pid,
						data_rcv->op1,data_rcv->op2,data_rcv->ops,*result);
	data_snd.type = data_rcv->pid;
	data_snd.result = *result;
	
	ret = msgsnd(msgq_sc,&data_snd,sizeof(int),data_rcv->pid);
	if (ret == -1) {
		perror("error in sending data:");
		exit(EXIT_FAILURE);
	}
	pthread_exit(result);
}
