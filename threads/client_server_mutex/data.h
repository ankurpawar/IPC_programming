#ifndef DATA_H
#define DATA_H

#define FIFO_C_S "fifo_c_s"
#define FIFO_S_C "fifo_s_c"
#define SEM_NAME1 "sem1"
#define SEM_NAME2 "sem2"
#define SEM_NAME3 "sem3"
#define SEM_NAME4 "sem4"

/*Client will send this structure to server.
  field pid is included so that result is sent to 
  intended process and not by any other process.Client 
  will fill it and Server will use it*/
typedef struct data
{
	int pid;
	int op1;
	int op2;
	char ops;
}DATA;

#endif
