/*
 *program to demonstrate simple application of message queue
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/types.h>

#define MSG_KEY 1097
#define MSG_LEN_MAX 32
#define MSG_TYPE 19

struct msg_struct
{
	long msg_type;
	char data[MSG_LEN_MAX];	
};

int main()
{
	int ret;
	struct msg_struct message1;
	int msg_type = MSG_TYPE;
	int msg_id;
	int loop = 1;

	message1.msg_type = MSG_TYPE;
 
	msg_id = msgget((key_t)MSG_KEY, 0666 | IPC_CREAT);
	if (msg_id == -1) {
		perror("error in creating message queue:");
		exit(EXIT_FAILURE);
	}

	while(loop) {
		ret = msgrcv(msg_id, &message1, MSG_LEN_MAX, msg_type, 0);
		if (ret == -1) {
			perror("error in reading from msgq\n");
			exit(EXIT_FAILURE);
		}
		printf("process2 reads = %32s\n",message1.data);
		if (strncmp(message1.data, "end", 3) == 0)
			loop = 0;
	}

	return EXIT_SUCCESS;
}
