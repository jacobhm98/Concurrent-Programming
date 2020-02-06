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
//method declarations
void swap(int, int);
void* quickSort(void *);
int partition();
void printArray();


//Global vars
int size;
int array[MAXSIZE];
//variable that keeps count of current number of threads active
int threadCounter = 0;
pthread_t workerId[MAXSIZE];
//pointers to pass to new thread
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
	sem_init(&sem, 1, 1);
	//populate the array
	size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
	for (int i = 0; i < size; i++){
		array[i] = rand() % 99;
	}
	printf("The array to sort: ");
	printArray();
	int pointers[2] = {0, size - 1};
	double starttime = read_timer();
	//create the first thread
	pthread_create(&workerId[0], NULL, quickSort, (void *) pointers); 
	threadCounter++;
	pthread_join(workerId[0], NULL);
	while (threadCounter != 0){
		//wait for all child threads to terminate
	}
	double endtime = read_timer();
	printf("The sorted array: ");
	printArray();
	printf("The execution time is %g sec\n", endtime - starttime);
	return 0;
}
//method that outputs the global array to stdout
void printArray(){
	printf("|");
	for (int i = 0; i < size; i++){
		printf("%d|", array[i]);
	}
	printf("\n");
}
//take arguments, put them in new mem location outside of scope of the thread that spawns this one
//and allow other threads to use global vars to spawn threads through incrementing semaphore
void * passArgs(void * arg){
	int * low = (int*) arg;
	int * high = low + 1;
	int * passed = (int*) malloc(2 * sizeof(int));
	*passed = *low;
	*(passed + 1) = *high;
	sem_post(&sem);
	quickSort((void*) passed);
	free(passed);
	
}
//the managing method of quicksort, spawning threads and doing recursive calls with the right pointers
void* quickSort(void * arg){
	int * low = (int*) arg;
	int * high = low + 1;
	
#ifdef DEBUG
	printf("index of low: %d\n", *low);
	printf("index of high: %d\n", *high);
#endif
//if the size of the subarray is not small enough we need to sort it, otherwise we have broken the problem down sufficiently
	if (*low < *high){
		//logic for partitioning the array
		int partitionElement = partition(*low, *high);
		//lock semaphore so only one thread passes arguments at a time
		sem_wait(&sem);
		args[0] = *low;
		args[1] = partitionElement - 1;
		pthread_create(&workerId[threadCounter], NULL, passArgs, (void *) args);
		threadCounter++;

		//solve the other partition recursively, so we only spawn need to spawn one new thread per partition
		int args1[2] = {partitionElement + 1, *high};
		quickSort((void *) args1);
	}
	else {
		threadCounter--;
		return NULL;
	}


}

int partition(int low, int high){
	//the rightmost index is the pivot element
	int pivot = high;
	int invPos = low - 1;		//index in array where we know all elements below it are smaller than the pivot: an invariant
#ifdef DEBUG
	printf("pivot : %d\n", array[pivot]);
	printf("invariant pos: %d\n", invPos);
	printArray();
#endif
	//iterate over the array, swapping elements positionally so that we divide the elements into smaller than pivot and larger than pivot
	//at the end we put the pivot in the spot after invPos where we know all elements are smaller than it to the left, and all elements
	//are larger than it to the right, effectively partitioning the array
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
	//return the position of the pivot element, we know we still need to sort the parts of the array to the left and to the right
	//of the pivot
	return invPos;
}

// A method to swap two elements given their positions in the array that we are to sort
void swap(int i1, int i2){
	int temp = array[i1];
	array[i1] = array[i2];
	array[i2] = temp;
	return;
}
