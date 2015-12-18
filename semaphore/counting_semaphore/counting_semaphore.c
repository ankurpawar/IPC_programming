/*
to compile the code on linux
~$ gcc -o binary_semaphore binary_semaphore.c -lpthread
This code creates 4 threads, and two semaphore one counting and other one
binary. Each thread calls the function get_food(), counting semaphore allows
3 thread to take true and one will get false from this function. So the output
will be like

thread 140324911208192, Ant got food = 1
thread 140324886030080, Dog got food = 0
thread 140324902815488, Bat got food = 1
thread 140324894422784, Cat got food = 1
thread 140324911208192, Ant got food = 1
thread 140324886030080, Dog got food = 1
thread 140324894422784, Cat got food = 1
thread 140324902815488, Bat got food = 0

Counting semaphore is allowing only 3 threads at a time to access the resource.
Resource is a int value. 

Binary semaphore is to avoid the race condition. It is not desirable that one 
thread is executing if condition and some other thread is incrementing count.
*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4 

int  count = 0;
sem_t binary_sema;
sem_t count_sema;

void *Ant(void *dummyPtr)
{
	pthread_t id;
	int ret, i = 0;
	while (i < 10) {
		sleep(1);	
		id = pthread_self();
		ret =  get_food();
		printf("thread %lu, Ant got food = %d\n",id, ret);
		i++;
	}
}

void *Bat(void *dummyptr)
{
	pthread_t id;
	int ret, i = 0;
	while (i < 10) {
		sleep(1);	
		id = pthread_self();
		ret =  get_food();
		printf("thread %lu, Bat got food = %d\n",id, ret);
		i++;
	}
}

void *Cat(void *dummyptr)
{
	pthread_t id;
	int ret, i = 0;
	while (i < 10) {
		sleep(1);	
		id = pthread_self();
		ret =  get_food();
		printf("thread %lu, Cat got food = %d\n",id, ret);
		i++;
	}
}

void *Dog(void *dummyptr)
{
	pthread_t id;
	int ret, i = 0;
	while(i < 10) {
		sleep(1);	
		id = pthread_self();
		ret =  get_food();
		printf("thread %lu, Dog got food = %d\n",id, ret);
		i++;
	}
}

int get_food(void)
{
	int ret = 0;
	sem_wait(&count_sema);
	sem_wait(&binary_sema);
	if (count >= 3) {
		ret = 0;
		count = 0;
	}
	else {
		count++;
		ret = 1;
	}
	sem_post(&binary_sema);
	sem_post(&count_sema);
	return ret;
}

int main()
{
	pthread_t thread_id[NTHREADS];
	int i, j;

	/*initialize binary semaphore with value one*/
	sem_init(&binary_sema, 0, 1);

	/*initialize count semaphore with value three. To allow 3 at once time*/
	sem_init(&count_sema, 0, 3); 
	
	pthread_create( &thread_id[0], NULL, Ant, NULL );
	pthread_create( &thread_id[1], NULL, Bat, NULL );
	pthread_create( &thread_id[2], NULL, Cat, NULL );
	pthread_create( &thread_id[3], NULL, Dog, NULL );
   
   	pthread_join( thread_id[0], NULL);
   	pthread_join( thread_id[1], NULL);
   	pthread_join( thread_id[2], NULL);
   	pthread_join( thread_id[3], NULL);
	
	sem_destroy(&binary_sema);
	sem_destroy(&count_sema);

	return 0;
}

