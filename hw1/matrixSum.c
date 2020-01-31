/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

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
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */
//#define DEBUG 1
pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_mutex_t bagBarrier;
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */ 
int numArrived = 0;       /* number who have arrived */
void contribute(int, int, int, int, int, int, int);
int bagOfTasks();
int rowToCompute = 0;

/* a reusable counter barrier */
void Barrier() {
  pthread_mutex_lock(&barrier);
  numArrived++;
  if (numArrived == numWorkers) {
    numArrived = 0;
    pthread_cond_broadcast(&go);
  } else
    pthread_cond_wait(&go, &barrier);
  pthread_mutex_unlock(&barrier);
}

/* timer */
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

double start_time, end_time; /* start and end times */
int size, stripSize;  /* assume size is multiple of numWorkers */
int sums[MAXWORKERS]; /* partial sums */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

//the global variables we update as program executes
int min = 10000000; //global variable used to store the minimum value of the matrix
int minPosX;
int minPosY;
int max = 0; //variable to hold maximum value in matrix
int maxPosX;
int maxPosY;
int sum = 0;

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&barrier, NULL);
  pthread_mutex_init(&bagBarrier, NULL);
  pthread_cond_init(&go, NULL);

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  stripSize = size/numWorkers;

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = rand()%99;
	  }
  }

  /* print the matrix */
#ifdef DEBUG
  for (i = 0; i < size; i++) {
	  printf("[ ");
	  for (j = 0; j < size; j++) {
	    printf(" %d", matrix[i][j]);
	  }
	  printf(" ]\n");
  }
#endif

  /* do the parallel work: create the workers */
  start_time = read_timer();
  
  for (l = 0; l < numWorkers; l++)
  pthread_create(&workerid[l], &attr, Worker, (void *) l);
  
  for (int k = 0; k < numWorkers; k++){
  	pthread_join(workerid[k], NULL);
  }
  end_time = read_timer(); 
  printf("The total sum of the matrix is: %d\n", sum);
  printf("The maximum value of the matrix is %d, located at %d, %d\n", max, maxPosX, maxPosY);
  printf("The minimum value of the matrix is %d, located at %d, %d\n", min, minPosX, minPosY);
  printf("The execution time is %g sec\n", end_time - start_time);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg) {
  long myid = (long) arg;
  int total, j, first, last, myMinPosX, myMinPosY, myMaxPosX, myMaxPosY;
  //Setting i to 0, and incrementing next line bagoftasks returns
  int i = bagOfTasks(); 
  //cannot have negative values so 0 is good, and maximum is less than 100 due to rand range
  int myMax = 0;
  int myMin = 10000000;
#ifdef DEBUG
  printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
#endif
  //while the number returned from bagoftasks is less than the matrix size, meaning we have rows left to compute
  while (i < size){
#ifdef DEBUG
  printf("this is the row being calculated: %d\n", i);
#endif
   /* sum values in my strip */
  total = 0;
    for (j = 0; j < size; j++){
      total += matrix[i][j];
      if (matrix[i][j] > myMax){	//updating minimum and maximum global variables as we find better fits in the matrix
      	myMax = matrix[i][j];
	myMaxPosX = i;		//saving the position max was found in
	myMaxPosY = j;
      }
      if (matrix[i][j] < myMin){
      	myMin = matrix[i][j];
	myMinPosX = i;
	myMinPosY = j;
      }
    }
  contribute(total, myMin, myMinPosX, myMinPosY, myMax, myMaxPosX, myMaxPosY); //updating global vars
  i = bagOfTasks(); 	//get new task to compute
  }
  return NULL;		//when theres no more tasks to do, terminate thread
}

void contribute(int partSum,int newMin, int newMinPosX, int newMinPosY, int newMax, int newMaxPosX, int newMaxPosY){
	pthread_mutex_lock(&barrier);		//updating of shared data structure (global vars), so we need to make sure only one
	sum += partSum;				//thread does this at a time
	if (newMin < min){
		min = newMin;
		minPosX = newMinPosX;
		minPosY = newMinPosY;
	}
	if (newMax > max){
		max = newMax;
		maxPosX = newMaxPosX;
		maxPosY = newMaxPosY;
	}
	pthread_mutex_unlock(&barrier);		//release lock
}
int bagOfTasks(){	//return next row to compute
	pthread_mutex_lock(&bagBarrier);	//we need to lock this as, again, we are updating shared data structures (rowtocompute)
	int assignedRow = rowToCompute;
	if (rowToCompute < size){
		rowToCompute++;
	}
	pthread_mutex_unlock(&bagBarrier);
	return assignedRow;	

}
