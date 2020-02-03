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
#define MAXSIZE 10000
#define MAXWORKERS 10
#define DEBUG 1

void swap(int, int);
void* quickSort(void *);
int partition();
void printArray();


//Global vars
int size;
int array[MAXSIZE];

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
	size = (argc > 1) ? atoi(argv[1]) : MAXSIZE;
	for (int i = 0; i < size; i++){
		array[i] = rand() % 99;
	}
	printArray();
	pthread_t workerId[size];
	int pointers[2] = {0, size - 1};
	printf("this is the size of the array %d\n", size);
	pthread_create(&workerId[0], NULL, quickSort, (void *) pointers); 
	pthread_join(workerId[0], NULL);


}
void printArray(){
	printf("|");
	for (int i = 0; i < size; i++){
		printf("%d|", array[i]);
	}
	printf("\n");
}
void* quickSort(void * arg){
	int * low = (int*) arg;
	int * high = low + 1;
	int partitionElement = partition(*low, *high);


}

int partition(int low, int high){
	int pivot = high;
	array[high] = 50;
	int invPos = low - 1;		//index in array where we know all elements below it are smaller than the pivot: an invariant
#ifdef DEBUG
	printf("pivot index: %d\n", pivot);
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
	return invPos;

}

// A method to swap two elements given their positions in the array that we are to sort
void swap(int i1, int i2){
	int temp = array[i1];
	array[i1] = array[i2];
	array[i2] = temp;
	return;
}
