/**@brief Program to test fork() 
 *
 * A simple program to create a child process using fork.
 * Parent wait for child to collect the exit status of child.  
 */
 
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	int ret_val, pid, pidc;
	int ret_val2;

	/*create a child*/
	pid = fork();
	if(pid < 0) {
		perror("fork failed\n");
		exit(EXIT_FAILURE);
	}
	if(pid > 0) { /*parent code*/
		printf("parent code pid=%d\n",getpid());
		/*wait for child to terminate and catch the exit code*/ 
		pidc = wait(&ret_val);
		printf("pidc = %d\n",pidc);
		if(pidc == -1) {
			perror("error in wait\n");
			exit(EXIT_FAILURE);
		}

		if(WIFEXITED(ret_val)) {
			/*child terminated sucessfully*/
			printf("child exited with status %d\n",WEXITSTATUS(ret_val));
		} else if (WIFSIGNALED(ret_val))
			printf("killed by signal %d\n", WTERMSIG(ret_val));
		else if (WIFSTOPPED(ret_val)) 
			printf("stopped by signal %d\n", WSTOPSIG(ret_val));
		else if (WIFCONTINUED(ret_val)) 
			printf("continued\n");
		
		exit(EXIT_SUCCESS);
	} else { /*child code*/
		printf("child pid=%d, parent pid=%d\n",getpid(), getppid());
		exit(EXIT_SUCCESS);
	}
	return EXIT_SUCCESS; 
}
