/**@brief Program to test fork() and execl()
 *
 *A simple program to create a child process using fork and replace
 *it using execl.
 *Parent opens a file and passes fd to child.Child writes some data to file
 *then parent waits for the child and reads data from the same file.The
 *child code will use execl() to replce it self with other program
 *that will write data to fd.fd will be passed as an command line
 *argument to program.  
 */
 
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	int ret_val,pid,pidc;
	int ret_val2;
	int fd;
	char buff[] = "hello once again to the world of linux";
	char read_buff[128];
	char *file_name = "test2";
	fd = open(file_name,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	printf("fd = %d\n",fd);
	if(fd < 0)
	{
		perror("error in oepning file");
		exit(EXIT_FAILURE);
	}

	/*create a child*/
	pid = fork();
	if(pid < 0) 
	{
		perror("fork failed\n");
		exit(EXIT_FAILURE);
	}
	if(pid > 0) /*parent code*/
	{
		printf("parent code pid=%d\n",getpid());
		/*wait for child to terminate and catch the exit code*/ 
		pidc = wait(&ret_val);
		printf("pidc = %d\n",pidc);
		if(pidc == -1)
		{
			perror("error in wait\n");
			exit(EXIT_FAILURE);
		}

		if(WIFEXITED(ret_val))
		{
			/*child terminated sucessfully*/
			printf("child exited with status %d\n",WEXITSTATUS(ret_val));
			/*parent has written in file so the see to the beginnin of file*/
			lseek(fd,0,SEEK_SET);
			ret_val = read(fd,read_buff,sizeof(read_buff));
			printf("ret_val =%d %s",ret_val,read_buff);
		} 
		else if (WIFSIGNALED(ret_val)) 
		{
			printf("killed by signal %d\n", WTERMSIG(ret_val));
		} 
		else if (WIFSTOPPED(ret_val)) {
			printf("stopped by signal %d\n", WSTOPSIG(ret_val));
		} 
		else if (WIFCONTINUED(ret_val)) {
			printf("continued\n");
		}
		/*close the file and exit*/
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		sprintf(read_buff,"%d",fd);
		printf("child pid=%d fd = %s\n",getpid(),read_buff);
		ret_val2 = execl("./write_data","write_data",file_name,read_buff,NULL);
		printf("come here when error\n");
		if(ret_val2 < 0)
		{
			perror("error execl\n");
			exit(EXIT_FAILURE);
		}
	}
	return EXIT_SUCCESS; 
}
