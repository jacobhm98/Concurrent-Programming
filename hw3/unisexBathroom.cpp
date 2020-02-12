
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#define DEBUG 1
#define DEFAULT_SIZE 1000
#define SHARED 1

//namespace
using std::cout;
using std::endl;

//function declarations
void * Work(void *);

//global variables n synchronization structures
sem_t lock;
sem_t mEnter;
sem_t wEnter;
int bathroomUsers = 0;


int main(int argc, char **argv){
	sem_init(&lock, SHARED, 1);
	sem_init(&mEnter, SHARED, 1);
	sem_init(&wEnter, SHARED, 1);
	srand(time(0));
	//set number of men and women from command line arguments, or if no args set them to default values
	int numMen;
	int numWomen;
	numMen = (argc > 1) ? atoi(argv[1]) : DEFAULT_SIZE;
	numWomen = (argc > 2) ? atoi(argv[2]) : DEFAULT_SIZE;
	long genderM = 0;
	long genderW = 1;
	pthread_t men[numMen];
	pthread_t women[numWomen];

	for (int i = 0; i < numMen; i++){
		pthread_create(&men[i], NULL, Work, (void *) genderM);
	}
	for (int i = 0; i < numWomen; i++){
		pthread_create(&women[i], NULL, Work, (void *) genderW);
	}
	for (int i = 0; i < numMen; i++){
		pthread_join(men[i], NULL);
	}
	for (int i = 0; i < numWomen; i++){
		pthread_join(women[i], NULL);
	}
	return 0;

}

void * Work(void * gender){
	long myGender = (long) gender;
	sem_wait(&lock);
	unsigned int workTime = rand() % 60;
	cout << "thread " << pthread_self() << " is boutta work for: " << workTime << " seconds" << endl;
	sem_post(&lock);
	sleep(workTime);
	cout << "thread " << pthread_self() << " needs to go to the bathroom!" << endl;
	if (gender == 0){
		sem_wait(&mEnter);

	}
	if (gender == 1){
		sem_wait(&wEnter);
	}
	return NULL;
}

void useBathroom(long gender){a
	if (gender == 0){
		if (bathroomUsers == 0){
			sem_wait(&wEnter);
		}
		sem_post(&mEnter);
	}
	else {
		if (bathroomUsers == 0){
			sem_wait(&mEnter);
		}
		sem_post(&wEnter);
	}
	bathroomUsers++;


}
