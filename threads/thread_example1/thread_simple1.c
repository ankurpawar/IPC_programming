/**
 *Create a thread and pass string as argument to 
 *created thread.
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *thread_func(void *arg);

int main()
{
	pthread_t thread1;
	void *ret_val;
	int ret;
	char *name="hello to the world";
	ret = pthread_create(&thread1,NULL,thread_func,(void*)name);
	if (ret != 0) {
		perror("error in creating thread");
		exit(EXIT_FAILURE);
	}

	printf("thread created\n");
	pthread_join(thread1,&ret_val);
	return EXIT_SUCCESS;
}

void *thread_func(void *arg)
{
	void *ret;
	printf("I am a thread function\n");
	printf("%s",(char*)arg);
	pthread_exit(ret);
}
