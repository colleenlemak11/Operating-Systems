/**
 * Colleen Lemak
 * Operating Systems Sect2
 * Dr. Y. Zhang
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];
/*define semaphores and mutex*/


int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
    /* Acquire Empty Semaphore */
	
	
	/* Acquire mutex lock to protect buffer */
	

	/* Release mutex lock and full semaphore */
	

	return 0;
}

int remove_item(buffer_item *item)
{
	/* Acquire Full Semaphore */
	 

	/* Acquire mutex lock to protect buffer */
 

	/* Release mutex lock and empty semaphore */
	 

	return 0;
}


int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	/*call atoi to get arguments */

 

	/* Create the producer and consumer threads */
 

	for(j = 0; j < consumerThreads; j++)
	{
 

	}

	/* Sleep for user specified time */
	sleep(sleepTime);
	return 0;
}

void *producer(void *param)
{
		 

	while(TRUE)
	{


	}

}

void *consumer(void *param)
{
	 
	 

	while(TRUE)
	{
 

	}
}
