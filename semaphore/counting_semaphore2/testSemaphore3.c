/*
Example taken from http://pages.cs.wisc.edu/~remzi/Classes/537/Fall2008/Notes/threads-semaphores.txt 

to compile this code on linux
~$ gcc -o testSemaphore3 testSemaphore3.c -lpthread

*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define MAX 3
sem_t empty;
sem_t full;

int buffer[MAX];
int fill = 0; 
int use  = 0;
int loops = 10;

void put(int value)
{
	buffer[fill] = value;    // line F1
	fill = (fill + 1) % MAX; // line F2
}

int get()
{
	int tmp = buffer[use];   // line G1
	use = (use + 1) % MAX;   // line G2
	return tmp;
}

void *producer(void *arg)
{
	int i;
	for (i = 0; i < loops; i++) {
		sem_wait(&empty);           // line P1 dec semaphore for empty buffer
		put(i);                     // line P2
		sem_post(&full);            // line P3 inc semaphore for full buffer
	}
}

void *consumer(void *arg)
{
	int i;
	for (i = 0; i < loops; i++) {
	sem_wait(&full);            // line C1 dec semaphore for full buffer
	int tmp = get();            // line C2
	sem_post(&empty);           // line C3 inc semaphore for empty buffer
	printf("%d\n", tmp);
	}
}

int main()
{
	pthread_t pid, cid;
	sem_init(&empty, 0, MAX); // MAX buffers are empty to begin with...
	sem_init(&full, 0, 0);    // ... and 0 are full
	pthread_create(&pid, NULL, producer, NULL); 
    	pthread_create(&cid, NULL, consumer, NULL); 
	pthread_join(pid, NULL); 
	pthread_join(cid, NULL); 
	return 0;
}
