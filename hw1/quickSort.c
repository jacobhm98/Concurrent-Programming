/* A program implementing the quicksort sorting algorithm concurrently
 * using the POSIX Thread (pthread) api.
 *
 * Usage: gcc quickSort.c -lpthread
 *  ./quickSort || ./quickSort sizeOfArrayToSort
 * If no int argument sizeOfArrayToSort is given, we sort an array of 10000 numbers.
 * The array contains numbers of random value between 0 and 98
 *
 * Written by Jacob Hedén Malm (jacmalm@kth.se)
 *
 */



#ifndef _REENTRANT
#define _REENTRANT 
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <semaphore.h>
#define MAXSIZE 10000
//#define DEBUG 1

void swap(int, int);
void* quickSort(void *);
int partition();
void printArray();


//Global vars
int size;
int array[MAXSIZE];
int threadCounter = 0;
pthread_t workerId[MAXSIZE];
pthread_mutex_t barrier;
int args[2];
sem_t sem;

//timer function taken from matrixSum.c
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

int main(int argc, char * argv[]){
	pthread_mutex_init(&barrier, NULL);
	sem_init(&sem, 1, 1);
	size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
	for (int i = 0; i < size; i++){
		array[i] = rand() % 99;
	}
	printf("The array to sort: ");
	printArray();
	int pointers[2] = {0, size - 1};
	double starttime = read_timer();
	pthread_create(&workerId[0], NULL, quickSort, (void *) pointers); 
	threadCounter++;
	pthread_join(workerId[0], NULL);
	while (threadCounter != 0){
	}
	double endtime = read_timer();
	printf("The sorted array: ");
	printArray();
	printf("The execution time is %g sec\n", endtime - starttime);
	return 0;
}
void printArray(){
	printf("|");
	for (int i = 0; i < size; i++){
		printf("%d|", array[i]);
	}
	printf("\n");
}

void * passArgs(void * arg){
	int * low = (int*) arg;
	int * high = low + 1;
	int * passed = (int*) malloc(2 * sizeof(int));
	*passed = *low;
	*(passed + 1) = *high;
	sem_post(&sem);
	quickSort((void*) passed);
	
}

void* quickSort(void * arg){
	int * low = (int*) arg;
	int * high = low + 1;
	
#ifdef DEBUG
	printf("index of low: %d\n", *low);
	printf("index of high: %d\n", *high);
#endif

	if (*low < *high){
		int partitionElement = partition(*low, *high);
		sem_wait(&sem);
		args[0] = *low;
		args[1] = partitionElement - 1;
		pthread_create(&workerId[threadCounter], NULL, passArgs, (void *) args);
		threadCounter++;

		int args1[2] = {partitionElement + 1, *high};
		quickSort((void *) args1);
	}
	else {
		threadCounter--;
		return NULL;
	}


}

int partition(int low, int high){
	pthread_mutex_lock(&barrier);
	int pivot = high;
	int invPos = low - 1;		//index in array where we know all elements below it are smaller than the pivot: an invariant
#ifdef DEBUG
	printf("pivot : %d\n", array[pivot]);
	printf("invariant pos: %d\n", invPos);
	printArray();
#endif
	for (int i = low; i < high; i++){
		if (array[i] <	array[pivot]){
			invPos++;
			swap(invPos, i);
		}
#ifdef DEBUG
		printArray();
#endif
	}
	invPos++;
	swap(invPos, pivot);
#ifdef DEBUG
	printArray();
#endif
	pthread_mutex_unlock(&barrier);
	return invPos;
}

// A method to swap two elements given their positions in the array that we are to sort
void swap(int i1, int i2){
	int temp = array[i1];
	array[i1] = array[i2];
	array[i2] = temp;
	return;
}
