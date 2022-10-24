/*
 * Colleen Lemak
 * Operating Systems
 * Project 4
 * 18 October 2022
 * To run: gcc -pthread -o thread project4.c, ./thread
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

// Global variables
int sorted[SIZE];
int unsorted[SIZE];

typedef struct sortParams
{
    int start;
    int mid;
    int end;
};

typedef struct insertParams
{
    int start;
    int end;
};

// Function prototypes
void *sorter(void *params);
void *merger(void *params);
void buildArray(int size);
void printArray(int *arr, int size);

// Implements sorting algorithm in place
void *sorter(void *params) {
    struct insertParams *insertPs = (struct insertParams*) params;
    int start = insertPs->start;
    int end = insertPs->end;

    int i = start;
    int j, item;

    while(i <= end) {
        item = unsorted[i];
        j = i - 1;
        while(j >= start && item < unsorted[j]) {
            unsorted[j+1] = unsorted[j];
            j--;
        }
        unsorted[j+1] = item;
        i++;
    }
}

// Merges two sorted halves into a new array
void *merger(void *params) {
    struct sortParams *sortPs = (struct sortParams*) params;
    int start = sortPs->start;
    int mid = sortPs->mid;
    int end = sortPs->end;

    int i = start;
    int j = mid;
    int pos = start;

    while(i < mid && j <= end) {
        if(unsorted[i] < unsorted[j])
            sorted[pos++] = unsorted[i++];
        else
            sorted[pos++] = unsorted[j++];
    }

    while(i < mid) {
        sorted[pos++] = unsorted[i++];
    }
    
    while(j <= end) {
        sorted[pos++] = unsorted[j++];
    }
}

void buildArray(int size) {
    srand(time(NULL));
    for(int i = 0; i < SIZE; i++) {
        unsorted[i] = rand() % 100;
    }
}

void printArray(int *arr, int size) {
    printf("**********Array Contents**********\n");
    for(int i = 0; i < SIZE; i++) {
        printf("%d\n", arr[i]);
    }
}

int main() {
    buildArray(SIZE);
    printf("\nUNSORTED:\n");
    printArray(unsorted, SIZE);

    int start1 = 0;
    int end1 = SIZE/2;
    int start2 = end1 + 1;
    int end2 = SIZE - 1;

    pthread_t threads[3]; // one merge thread, two sorting threads

    // prepare insertion params for thread
    struct insertParams iParams[2];
    iParams[0].start = start1;
    iParams[0].end = end1;
    pthread_create(&threads[0], NULL, sorter, &iParams[0]);

    iParams[1].start = start2;
    iParams[1].end = end2;
    pthread_create(&threads[1], NULL, sorter, &iParams[1]);

    // wait until sorting threads terminate
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // prepare sorting params for thread
    struct sortParams sParams;
    sParams.start = start1;
    sParams.mid = start2;
    sParams.end = end2;
    pthread_create(&threads[2], NULL, merger, &sParams);

    pthread_join(threads[2], NULL);
    printf("\nSORTED:\n");
    printArray(sorted, SIZE);

    return 0;
}


