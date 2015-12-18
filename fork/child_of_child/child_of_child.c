/*
 *Program to make below given hirerarchy using fork
 *  parent
 *       |
 *       child
 *           |
 *           child
 *               |
 *               child  
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define NUM_CHILD 3

int main()
{
	int ret_pid;
	int ret_val;
	int i;

	for (i = 0;i < NUM_CHILD;i++) {
		ret_pid = fork();
		if (ret_pid == -1) {
			perror("error in fork:");
			exit(EXIT_FAILURE);
		}

		
		if (ret_pid > 0) {
			/*parent code*/
			printf("parent code, pid = %d,parent pid = %d\n",getpid(),getppid());
			break;
		} else {
			/*child code*/
			printf("child code, pid = %d,parent pid = %d\n",getpid(),getppid());
			/*last one will not have any child,so exit from here*/
			if (i == NUM_CHILD-1)
				exit(EXIT_SUCCESS);			
		}
	}

	ret_pid = wait(&ret_val);
	if(ret_pid == -1) {
		perror("error in wait:");
		exit(EXIT_FAILURE);
	}
	printf("terminated pid = %d,status = %d\n",ret_pid,ret_val);
	return EXIT_SUCCESS;
}
