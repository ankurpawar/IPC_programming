/**@brief Program to test fork()
 *
 *A simple program to create a child process using fork.
 *Parent opens a file and child writes some data to file
 *then parent waits for the child and reads data from the
 *same file
 */
 
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	int ret_val,pid,pidc;
	int fd;
	char buff[] = "hello to the world of linux";
	char read_buff[32];
	fd = open("test",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
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
	if(pid > 0)
	{
		/*wait for child to terminate and catch the exit code*/ 
		pidc = wait(&ret_val);
		if(WIFEXITED(ret_val))
		{
			printf("child exited with status %d\n",WEXITSTATUS(ret_val));
			/*parent has written in file so the see to the beginnin of file*/
			lseek(fd,0,SEEK_SET);
			ret_val = read(fd,read_buff,sizeof(read_buff));
			printf("ret_val =%d %s",ret_val,read_buff);
		}
		/*close the file and exit*/
		close(fd);
		exit(EXIT_SUCCESS);
	}
	else
	{
		/*write some data to file*/
		if(write(fd,buff,sizeof(buff) < 0)
		{
			perror("error in writing file\n");
			exit(EXIT_FAILURE);
		} 
		printf("data written\n");
	}
	return EXIT_SUCCESS; 
}
