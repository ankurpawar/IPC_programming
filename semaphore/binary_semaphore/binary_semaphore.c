/*
to compile the code on linux
~$ gcc -o binary_semaphore binary_semaphore.c -lpthread

*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 10

void *thread_function(void *);

int  counter = 0;
sem_t binary_sema;

int main()
{
	pthread_t thread_id[NTHREADS];
	int i, j;

	sem_init(&binary_sema, 0, 1); /*initialize smaphor with value one*/
	
	for(i=0; i < NTHREADS; i++) 
		pthread_create( &thread_id[i], NULL, thread_function, NULL );
   	
	for(j=0; j < NTHREADS; j++)
   		pthread_join( thread_id[j], NULL);

	/* Now that all threads are complete I can print the final result.     */
	/* Without the join I could be printing a value before all the threads */
	/* have been completed.                                                */
	printf("Final counter value: %d\n", counter);
	return 0;
}

void *thread_function(void *dummyPtr)
{
	printf("Thread number %lu\n", pthread_self());
	sem_wait(&binary_sema); /*dec the binary semaphore*/
	counter++;
	sem_post(&binary_sema); /*inc the binary semaphore*/
}
