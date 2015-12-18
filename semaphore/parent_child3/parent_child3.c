/**
 *
 *Demonstrate simple IPC using semaphore sync
 *here 3 requesting client send operands and operator
 *server passes to one processing client 
 *without dec op on semaphore.this will only do inc
 *op in parent and initial value will be 0.when child 
 *try to dec it will block.
 */
#include<stdio.h>
#include<stdlib.h>
#include<linux/sem.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define NUM_REQ_CLIENT 3
#define NUM_CALC_CLIENT 3
#define SEM_KEY 9876

typedef struct data
{
	int op1;
	int op2;
	char ops;
}DATA;

int main()
{
	int ret_val;
	int ret_pid;
	int i;
	int req_pipe[2];
	int calc_pipe[2];
	int sem_id ;
	DATA d[3] = { {8,2,'+'} , {14,9,'-'} , {5,6,'*'} };
	DATA a[3];
	union semun semun_var1;
	union semun semun_var2;
	struct sembuf sem_op1[3];
	struct sembuf sem_op2[3];
	
	memset(sem_op1,0,sizeof(struct sembuf)*3);
	memset(sem_op2,0,sizeof(struct sembuf)*3);

	sem_op1[0].sem_num = 0;
	sem_op1[0].sem_op = -1;
	sem_op1[0].sem_flg = SEM_UNDO;

	sem_op1[1].sem_num = 0;
	sem_op1[1].sem_op = 0;
	sem_op1[1].sem_flg = SEM_UNDO;
	
	sem_op1[2].sem_num = 0;
	sem_op1[2].sem_op = 1;
	sem_op1[2].sem_flg = SEM_UNDO;
	 
	memcpy(sem_op2,sem_op1,sizeof(struct sembuf)*3);
	
	sem_op2[0].sem_num = 1;
	sem_op2[1].sem_num = 1;
	sem_op2[2].sem_num = 1;
	
	/*pipe for requesting child*/
	ret_val = pipe(req_pipe);
	if(ret_val == -1)
	{
		perror("error in creating pipe:");
		exit(EXIT_FAILURE);
	}

	/*pipe for processing child*/
	ret_val = pipe(calc_pipe);
	if(ret_val == -1)
	{
		perror("error in creating pipe:");
		exit(EXIT_FAILURE);
	}

	sem_id = semget(SEM_KEY,3,IPC_CREAT|0777);
	if(sem_id == -1)
	{
		perror("error in semget:");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while(i < NUM_REQ_CLIENT)
	{
		int ret;
		ret_val = fork();
		if(ret_val == -1)
		{
			perror("error in fork:");
			exit(EXIT_FAILURE);
		}
		/*initialize semaphore1 with 1*/
		semun_var1.val = 1;	
		ret = semctl(sem_id,0,SETVAL,semun_var1);
		if(ret == -1)
		{
			perror("error in semctl:");
			exit(EXIT_FAILURE);
		}
		/*parent*/
		if(ret_val > 0) 
		{
			read(req_pipe[0],&a[i],sizeof(DATA));
			//printf("data a.op1=%d,a.op2=%d,a.ops=%c \n",a[i].op1,a[i].op2,a[i].ops);
		}
		else /*child*/
		{
			int res = 0;
			printf("pid=%d i =%d d.op1=%d,d.op2=%d,d.ops=%c \n ",getpid(),i,d[i].op1,d[i].op2,d[i].ops);
			write(req_pipe[1],&d[i],sizeof(DATA));
			ret = semctl(sem_id,1,GETVAL);
			if(ret == -1)
			{
				perror("error in semctl:");
				exit(EXIT_FAILURE);
			}
			printf("\n#child semval=%d\n",ret);
			/*wait for semaphore1 to become 0 then inc here*/
			if(semop(sem_id,sem_op1+1,2) == -1)
			{
				perror("error in semop:");
				exit(EXIT_FAILURE);
			}
			read(req_pipe[0],&res,sizeof(int));
			printf("pid=%d result = %d\n",getpid(),res);
			ret = semctl(sem_id,1,GETVAL);
			if(ret == -1)
			{
				perror("error in semctl:");
				exit(EXIT_FAILURE);
			}
			printf("pid=%d child semval=%d\n",getpid(),ret);
			//close(req_pipe[0]);
			//sleep(1);
			exit(EXIT_SUCCESS);
		}
		i++;
	}

	for(i = 0;i < NUM_CALC_CLIENT;i++)
	{
		int ret;
		ret_val = fork();
		if(ret_val == -1)
		{
			perror("error in fork:");
			exit(EXIT_FAILURE);
		}
		semun_var2.val = 1;
		/*initialize semaphore2 with value 1*/
		ret = semctl(sem_id,1,SETVAL,semun_var2);
		if(ret == -1)
		{
			perror("error in semctl:");
			exit(EXIT_FAILURE);
		}
		
		/*parent*/	
		if(ret_val > 0)
		{
			int res = 0;
			int ret = 0;
			write(calc_pipe[1],&a[i],sizeof(DATA));
			printf("parent pid=%d a.op1=%d,a.op2=%d,a.ops=%c\n",getpid(),a[i].op1,a[i].op2,a[i].ops);
			/*decrement semaphore2 here,before read*/
			if(semop(sem_id,sem_op2+1,2) == -1)
			{
				perror("error in semop:");
				exit(EXIT_FAILURE);
			}
			read(calc_pipe[0],&res,sizeof(int));
			write(req_pipe[1],&res,sizeof(int));
			ret = semctl(sem_id,0,GETVAL);
			if(ret == -1)
			{
				perror("error in semop:");
				exit(EXIT_FAILURE);
			}
			printf("pid=%d semval ret = %d i =%d \n",getpid(),ret,i);
			/*dec semaphore1 here after write*/
			if(semop(sem_id,&sem_op1[0],1) == -1)
			{
				perror("error in semop:");
				exit(EXIT_FAILURE);
			}
			
		}
		else /*child*/
		{
			DATA b;
			int res = 0;
			int ret = 0;
			int j = 0;
			b.op1=0;
			b.op2=0;
			b.ops='!';
			ret = semctl(sem_id,2,GETVAL);
			if(ret == -1)
			{
				perror("error in semctl\n");
				exit(EXIT_FAILURE);	
			}
			ret = read(calc_pipe[0],&b,sizeof(DATA));
			printf("val read = %d pid=%d b.op1=%d,b.op2=%d,b.ops=%c\n",ret,getpid(),b.op1,b.op2,b.ops); 
			switch(b.ops)
			{
				case'+':
					res = b.op1 + b.op2;
					break;
				case'-':
					res = b.op1 - b.op2;
					break;
				case'*':
					res = b.op1 * b.op2;
					break;
				default:
					res = 0;
					break;
			}
			//printf("processing result = %d\n",res);
			write(calc_pipe[1],&res,sizeof(int));
			/*dec semaphore2 here*/
			if(semop(sem_id,&sem_op2[0],1) == -1)
			{
				perror("error in semop:");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
	}
	printf("waiting for child\n");
	for(i = 0;i < (NUM_REQ_CLIENT+NUM_CALC_CLIENT);i++)
	{
		ret_pid = wait(&ret_val);
		if(ret_pid == -1)
		{
			perror("error in wait");
			exit(EXIT_FAILURE);
		}
		if(WIFEXITED(ret_val))
		{
			printf("status=%d\n",WEXITSTATUS(ret_val));
		}
	
	}

	ret_val = semctl(sem_id,0,IPC_RMID,semun_var1);
	if(ret_val == -1)
	{
		perror("error in removing semaphore:");
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;	
}
