/*
 * Multi access threaded queue with multiple threads writing and multiple
 * threads reading from the queue. Posix message queue are thread safe , no 
 * mutex is required. Any reader thread can receive message from any writer
 * because there is no message id.
 * to compile the code
 * gcc -o threaded_queue threaded_queue.c -lpthread -lrt
 * output
 *  this is write thread = 1
 *  this is write thread = 2
 *  this is write thread = 0
 *  this is read thread=1, message is =hello from writer thread0, prio=0
 *  this is read thread=2, message is =hello from writer thread2, prio=2
 *  this is read thread=0, message is =hello from writer thread1, prio=1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

#define MAX_THREAD	3
#define MESSAGE_Q_NAME	"/knot" 
#define BUFF_SIZE	64
#define MSG_SIZE	128
#define MAX_MSG		10

mqd_t message_q; /*only one queue*/

void *write_func(void *arg)
{
	unsigned int num = *(unsigned int*) arg;
	char buff[MSG_SIZE];
	printf("this is write thread = %d\n", num);
	snprintf(buff, BUFF_SIZE,"hello from writer thread%u", num); /*copy data to buffer to be sent*/
	if(mq_send(message_q, buff, MSG_SIZE, num) == -1) {
		perror("error in writing data\n");
		exit(EXIT_FAILURE);
	}
}


void *read_func(void *arg)
{
	unsigned int num = *(unsigned int*) arg;
	unsigned int prio = 0;
	int i;
	char buff[MSG_SIZE];
	i = mq_receive(message_q, buff, MSG_SIZE, &prio);
	if (i == -1) {
		perror("error in reading data\n");
		exit(EXIT_FAILURE);
	}
	printf("this is read thread=%d, message=%s, prio=%u\n", num, buff, prio);
}


int main()
{
	int i;
	pthread_t write_thread[MAX_THREAD];
	pthread_t read_thread[MAX_THREAD];
	unsigned int ids[] = {0, 1, 2};
	struct mq_attr message_q_attr;
	
	/*creating posix message queue*/
	message_q_attr.mq_maxmsg = MAX_MSG; /*maximum messages in queue at a time*/ 
	message_q_attr.mq_msgsize = MSG_SIZE; /*maximum size of message in a queue*/
	message_q_attr.mq_flags = 0;

	message_q = mq_open(MESSAGE_Q_NAME, O_RDWR|O_CREAT, 0664, &message_q_attr);
	if (message_q == -1) {
		perror("error in creating queue\n");
		exit(EXIT_FAILURE);
	}

	/*create write threads*/
	for(i = 0; i < MAX_THREAD; i++) {
		if (pthread_create(&write_thread[i], NULL, write_func, &ids[i]) != 0) {
			printf("error in creating threads\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/*create read threads*/
	for(i = 0; i < MAX_THREAD; i++) {
		if (pthread_create(&read_thread[i], NULL, read_func, &ids[i]) != 0) {
			printf("error in creating threads\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/*join the write threads*/
	for(i = 0; i < MAX_THREAD; i++) {
		if (pthread_join(write_thread[i], NULL) != 0) {
			printf("error in joining threads\n");
			exit(EXIT_FAILURE);
		}
	}

	
	/*join the read threads*/
	for(i = 0; i < MAX_THREAD; i++) {
		if (pthread_join(read_thread[i], NULL) != 0) {
			printf("error in joining threads\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/*close message queue*/
	mq_close(message_q);
	/*destroy the queue*/
	mq_unlink(MESSAGE_Q_NAME);
	return 0;
}
