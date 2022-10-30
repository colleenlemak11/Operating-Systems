/**
 * Colleen Lemak
 * Operating Systems Sect2
 * Dr. Y. Zhang
 * 2 November 2022
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

// initialize buffer, mutex, semaphores
buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t lock;
int nextin = 0, nextout = 0, insertPointer = 0, removePointer = 0;
sem_t empty, occupied;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
	// decrement num empty slots in buffer
	sem_wait(&empty);

	// lock during insert to protect buffer
	pthread_mutex_lock(&lock);
	
	// circular buffer redirects insert index
	if(++nextin >= BUFFER_SIZE) {
		nextin = 0;
		buffer[nextin] = item;
	}
	else {
		buffer[nextin] = item;
		nextin++;
		printf("buffer[nextin]:%d", buffer[nextin]);
	}

	// release lock and update occupied spots
	pthread_mutex_unlock(&lock);
	sem_post(&occupied);

	return 0;
}

int remove_item(buffer_item *item)
{
	/* Acquire Full Semaphore */
	sem_wait(&occupied);
	 
	// lock during insert to protect buffer
	pthread_mutex_lock(&lock);
	
	if(++nextout >= BUFFER_SIZE) {
		nextout = 0;
	}
	while(nextout > 0) {
		buffer[nextout] = buffer[nextout - 1];
		nextout--;
		printf("buffer[nextout]:%d", buffer[nextout]);
	}
	
	// release lock and update empty spots
	pthread_mutex_unlock(&lock);
	sem_post(&empty);
	
	return 0;
}

int main(int argc, char *argv[])
{
	printf("we're in main!");
	int sleepTime, producerThreads, consumerThreads;
	int i, j;
	pthread_t producer[producerThreads];
	pthread_t consumer[consumerThreads];

	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&occupied, 0, 0);
	pthread_join(&producer, NULL);
	pthread_join(&consumer, NULL);

	// check valid entry formatting
	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	// get cmd line arguments
	sleepTime = atoi(argv[1]);
	printf("Sleep time:", sleepTime);
	producerThreads = atoi(argv[2]);
	printf("Producer threads:", producerThreads);
	consumerThreads = atoi(argv[3]);
	printf("Consumer threads:", consumerThreads);

	// create producer and consumer threads
	for(int i = 0; i < producerThreads; i++) {
		pthread_create(&producer[i], NULL, producer, NULL);
	}

	for(int j = 0; j < consumerThreads; j++) {
		pthread_create(&consumer[j], NULL, consumer, NULL);
	}

	// sleep then exit program
	sleep(sleepTime);
	pthread_exit(NULL);

	return 0;
}

void *producer(void *param)
{
	buffer_item item = -1;
	int upper = 10, lower = 0, sleepTime = 2;

	while(TRUE)
	{
		sleep(sleepTime);
		sem_wait(&empty);
		item = (rand() % (upper - lower + 1)) + lower;
		if(insert_item(&item)) {
			fprintf(stderr, "report error condition, Producer.");
		}
		else {
			printf("Producer produced %d\n", item);
		}
		sem_post(&occupied);
	}
}

void *consumer(void *param)
{
	 buffer_item item = -1;
	 int sleepTime = 2;
	 
	while(TRUE)
	{
		sleep(sleepTime);
		sem_wait(&occupied);
		if(remove_item(&item)) {
			fprintf(stderr, "report error condition, Consumer.");
		}
		else {
			printf("Consumer consumed %d\n", item);
		}

		printf("Removed item:%d", item);
		sem_post(&empty);
	}
}