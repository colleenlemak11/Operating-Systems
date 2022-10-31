/**
 * Colleen Lemak
 * Operating Systems Sect2
 * Dr. Y. Zhang
 * 2 November 2022
 * To use: gcc -pthread -o thread buffer.c
 * Execution: ./thread 1 2 2
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

// initialize buffer, mutex, semaphores
buffer_item buffer[BUFFER_SIZE];
sem_t empty, occupied;
pthread_mutex_t mutex;
pthread_t tid;
int counter;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{	
	// circular buffer redirects insert index
	if(counter >= BUFFER_SIZE) {
		counter = 0;
		buffer[counter] = item;
		counter++;
	}
	else {
		buffer[counter] = item;
		counter++;
	}
	return 0;
}

int remove_item(buffer_item *item)
{
	// Remove an item if buffer is not empty
	if(counter > 0) {
		*item = buffer[(counter - 1)];
		counter--;
	}
	else {
		return -1;
	}
	return 0;
}

void initData() {
	pthread_mutex_init(&mutex, NULL);
	sem_init(&occupied, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	counter = 0;
}

int main(int argc, char *argv[])
{
	// check valid entry formatting
	if(argc != 4) {
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	// get cmd line arguments
	int sleepTime = atoi(argv[1]);
	int producerThreads = atoi(argv[2]);
	int consumerThreads = atoi(argv[3]);

	// initialize data before use
	initData();

	// create producer and consumer threads
	for(int i = 0; i < producerThreads; i++) {
		pthread_create(&tid, NULL, producer, NULL);
	}
	for(int j = 0; j < consumerThreads; j++) {
		pthread_create(&tid, NULL, consumer, NULL);
	}

	// sleep then exit program
	sleep(sleepTime);
	pthread_exit(NULL);

	return 0;
}

void *producer(void *param)
{
	buffer_item item;
	int sleepTime = 2;

	// producer inserts items into buffer -> producer
	while(TRUE) {
		sleep(sleepTime);
		item = rand() % 100; // insert rand item

		// decrement empty and lock thread mutex
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		if(insert_item(item)) {
			fprintf(stderr, "report error condition, Producer.\n");
		}
		else {
			printf("Producer produced %d\n", item);
		}

		// increment occupied and unlock mutex
		pthread_mutex_unlock(&mutex);
		sem_post(&occupied);
	}
}

void *consumer(void *param)
{
	buffer_item item;
	int sleepTime = 2;
	
	// consumer removes items from buffer -> consumer
	while(TRUE) {
		sleep(sleepTime);
		// decrement occupied and lock thread
		sem_wait(&occupied);
		pthread_mutex_lock(&mutex);

		if(remove_item(&item)) {
			fprintf(stderr, "report error condition, Consumer.\n");
		}
		else {
			printf("Consumer consumed %d\n", item);
		}

		// increment empty spots and unlock mutex
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}