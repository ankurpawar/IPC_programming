#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/ipc.h>
#include"data.h"

int main()
{
	int ret;
	MSG_CLIENT data_snd;
        MSG_SERVER data_rcv;

	int msgq_cs;
	int msgq_sc;

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

	data_snd.type = 1;
	data_snd.pid = getpid();
	data_snd.op1 = 8;
	data_snd.op2 = 3;
	data_snd.ops = '*';
	
	ret = msgsnd(msgq_cs,&data_snd,sizeof(MSG_CLIENT) - sizeof(long),1);
	if (ret == -1) {
		perror("error in sending:");
		exit(EXIT_FAILURE);
	}
	printf("pid = %d message sent\n",data_snd.pid);	

	ret = msgrcv(msgq_sc,&data_rcv,sizeof(MSG_SERVER),data_snd.pid,0);
	if (ret == -1) {
		perror("error in receiving:");
		exit(EXIT_FAILURE);
	}
	printf("pid = %d result = %d \n",data_snd.pid,data_rcv.result);
	return EXIT_SUCCESS;
}
