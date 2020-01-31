/* A program implementing the quicksort sorting algorithm concurrently
 * using the POSIX Thread (pthread) api.
 *
 * Usage: ./quickSort || ./quickSort sizeOfArrayToSort
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
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000
#define MAXWORKERS 10
//#define DEBUG 1

void swap(int, int);
void* quickSort();
void partition();


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


}

void* quickSort(int * pointers){
	int low = pointers[0];
	int high = pointers[1];

}

void partition(int low, int high){
	int pivot = array[high];
	int invPos = 0;		//index in array where we know all elements below it are smaller than the pivot: an invariant
	for (int i = low; i < high; i++){
		
	}

}

// A method to swap two elements given their positions in the array that we are to sort
void swap(int i1, int i2){
	int temp = array[i1];
	array[i1] = array[i2];
	array[i2] = temp;
	return;
}
