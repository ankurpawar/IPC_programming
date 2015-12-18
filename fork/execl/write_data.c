/*@brief Program to write data to file
 *
 *This program takes file fd as commncd line argument
 *and writes to some data to that file.If file doesn't
 *exists no new file is created. 
 */

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char *argv[])
{
	int fd,ret_val;
	char file_name[32] = {0};
	char fd_c[4] = {0};
	char buff[]="if n fork are placed subsequently in code then (2^n)-1 children will be created";
	if(argc < 2)
	{
		printf("too few arguments .Pass filename fd\n");
		exit(EXIT_FAILURE);
	}
	fd = atoi(argv[2]);
	printf("fd = %d,",fd);
	strcpy(file_name,argv[1]);
	printf("filename = %s\n",file_name);
	/*open file*/
	ret_val =  open(file_name,O_RDONLY);
	if(ret_val < 0)
	{
		perror("error in opening file\n");
		exit(EXIT_FAILURE);
	}
	/*write some data*/
	ret_val = write(fd,buff,sizeof(buff));
	if(ret_val < 0)
	{
		perror("error in writing file\n");
		exit(EXIT_FAILURE);
	}
	/*close(fd);*/

	return EXIT_SUCCESS;
}
