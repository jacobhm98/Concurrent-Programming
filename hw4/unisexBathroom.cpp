/* A program which attempts to solve the unisex bathroom problem in a fair way. Given x and y workers of male and female genders, can we
 * coordinate their access to a single bathroom so that men and women never use the bathroom at the same time, and so that a person waiting
 * in line is guaranteed to be allowed to use the bathroom eventually? A worker is represented by a Posix Thread, and work time is represented
 * by sleeping a random amount of time. Bathroom use is represented by sleeping a smaller random amount of time. All synchronization is
 * accomplished using a monitor which is implemented with 4 counters, 2 condition variables, and a mutex lock
 *
 * Compile: There is a script provided, ./makeFile, which compiles this and the monitor program into ./unisexBathroom.
 * Execute: ./unisexBathroom numMen numWomen
 * For a more verbose updating of events, define DEBUG to 1
 * If no args are given, the program will be run with 1000 men and 1000 women
 *
 * Authored by Jacob Hedén Malm (@jacobhm98) for Concurrent Programming at KTH
 * 20/02/2020
 */

#ifndef _REENTRANT
#define _REENTRANT
#endif
#include "Monitor.h"
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#define DEBUG 0
#define DEFAULT_SIZE 1000
#define SHARED 1

//namespace
using std::cout;
using std::endl;

//function declarations
void * mWork(void *);
void * wWork(void *);
void useBathroom(long);

//for generating the random work and bathroom time
pthread_mutex_t mutex;
Monitor monitor;

int main(int argc, char **argv){
	pthread_mutex_init(&mutex, NULL);
	srand(time(0));
	//set number of men and women from command line arguments, or if no args set them to default values
	int numMen;
	int numWomen;
	numMen = (argc > 1) ? atoi(argv[1]) : DEFAULT_SIZE;
	numWomen = (argc > 2) ? atoi(argv[2]) : DEFAULT_SIZE;
	pthread_t men[numMen];
	pthread_t women[numWomen];

	for (long i = 0; i < numMen; i++){
		pthread_create(&men[i], NULL, mWork, (void *) i);
	}
	for (long i = 0; i < numWomen; i++){
		pthread_create(&women[i], NULL, wWork, (void *) (i));
	}
	for (int i = 0; i < numMen; i++){
		pthread_join(men[i], NULL);
	}
	for (int i = 0; i < numWomen; i++){
		pthread_join(women[i], NULL);
	}
	return 0;

}

void * mWork(void * id){
	long myId = (long) id;
	while (true){
		//atomically generate worktime and go to work, as we cannot have concurrent calls to rand
		pthread_mutex_lock(&mutex);
		unsigned int workTime = rand() % 60;
#if DEBUG == 1
		cout << "Man number  " << myId << " is boutta work for: " << workTime << " seconds" << endl;
#endif
		pthread_mutex_unlock(&mutex);
		sleep(workTime);
		//request acces from monitor 
		monitor.manEnter(myId);
		useBathroom(myId);
		monitor.manExit(myId);
	}
	return NULL;
}
void * wWork(void * id){
	long myId = (long) id;
	while (true){
		pthread_mutex_lock(&mutex);
		unsigned int workTime = rand() % 60;
#if DEBUG == 1
		cout << "Woman number " << myId << " is boutta work for: " << workTime << " seconds" << endl;
#endif
		pthread_mutex_unlock(&mutex);
		sleep(workTime);
		//request acces from monitor
		monitor.womanEnter(myId);
		useBathroom(myId);
		monitor.womanExit(myId);
	}		
	return NULL;
}

void useBathroom(long id){
	pthread_mutex_lock(&mutex);
	unsigned int bathroomTime = rand() % 5;
	pthread_mutex_unlock(&mutex);
	sleep(bathroomTime);
	return;
}
