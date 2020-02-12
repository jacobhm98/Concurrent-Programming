
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#define DEBUG 1
#define DEFAULT_SIZE 1000
using std::cout;
using std::endl;

void * Work(void *);


int main(int argc, char **argv){
	int numMen;
	int numWomen;
	numMen = (argc > 1) ? atoi(argv[1]) : DEFAULT_SIZE;
	numWomen = (argc > 2) ? atoi(argv[2]) : DEFAULT_SIZE;
	int genderM = 0;
	int genderW = 1;
	pthread_t men[numMen];
	pthread_t women[numWomen];

	for (int i = 0; i < numMen; i++){
		pthread_create(&men[i], NULL, Work, (void *) genderM);
	}
	for (int i = 0; i < numWomen; i++){
		pthread_create(&women[i], NULL, Work, (void *) genderW);
	}
	return 0;

}

void * Work(void * gender){
	
}
