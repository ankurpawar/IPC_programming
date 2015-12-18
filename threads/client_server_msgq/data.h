#ifndef DATA_H
#define DATA_H

#define MSGQ_C_S 1201
#define MSGQ_S_C 1210
#define SEM_NAME1 "sem_1"
#define SEM_NAME2 "sem_2"
#define SEM_NAME3 "sem_3"
#define SEM_NAME4 "sem_4"

/*Client will send this structure to server.
 *field pid is included so that result is sent to 
 *intended process and not by any other process.Client 
 *will fill it and Server will use it
 *Send stuct must have type field as 1,to idetify 
 *it as request.pid field to recognize from which client it is received. 
 */
typedef struct msg_client 
{
	long type;
	int pid;
	int op1;
	int op2;
	char ops;
}MSG_CLIENT;

typedef struct msg_server 
{
	long type;
	int result;
}MSG_SERVER;
#endif
