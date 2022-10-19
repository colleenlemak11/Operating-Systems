/*
 * Colleen Lemak
 * Operating Systems
 * Project 4
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct
{
	int from_index;
	int to_index;
} parameters;

int main (int argc, const char * argv[]) 
{
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	/* establish the first sorting thread */
	//1. call malloc to allocate a �parameters�
	//2. use �parameters� to specify the first half of the array
      //3. create the first thread
	

	/* establish the second sorting thread */
	//1. call malloc to allocate a �parameters�
	//2. use �parameters� to specify the first half of the array
      //3. create the second thread
  
	
	/* now wait for the 2 sorting threads to finish */
	// use ptheread_join; wait for 2 sorting threads to finish 
	

	/* establish the merge thread */
	//reuse �parameters� to hold the beginning index in each half
	//create the third thread: merge 
	
	/* wait for the merge thread to finish */
	 


	/* output the sorted array */
	 
	
    return 0;
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
	parameters* p = (parameters *)params;
	
	// Sorting algorithm here. It can be any sorting algorithm.

	pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{
	parameters* p = (parameters *)params;
	
	//reuse �parameters� to hold the first index in each half
	//merge two sorted sublist to the arry result
	
	pthread_exit(0);
}


