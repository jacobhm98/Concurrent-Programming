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
void * mWork(void *);
void * wWork(void *);
void useBathroom(long);

//global variables n synchronization structures
sem_t lock;
sem_t mEnter;
sem_t wEnter;
int menWaiting = 0;
int womenWaiting = 0;
int menUsingBathroom = 0;
int womenUsingBathroom = 0;

int main(int argc, char **argv){
	sem_init(&lock, SHARED, 1);
	sem_init(&mEnter, SHARED, 0);
	sem_init(&wEnter, SHARED, 0);
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
		pthread_create(&women[i], NULL, wWork, (void *) (i + numMen));
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
		sem_wait(&lock);
		unsigned int workTime = rand() % 60;
		cout << "worker " << myId << " is boutta work for: " << workTime << " seconds" << endl;
		sem_post(&lock);
		sleep(workTime);
		cout << "worker " << myId << " needs to go to the bathroom!" << endl;
		
		//wait for bathroom to be empty of women and join the cue
		sem_wait(&lock);
		if (womenUsingBathroom > 0){
			menWaiting++;
			sem_post(&lock);
			sem_wait(&mEnter);
		}
		menUsingBathroom++;
		//empty cue, pass the baton
		if (menWaiting > 0){
			menWaiting--;
			sem_post(&mEnter);
		}
		sem_post(&lock); //why?
		useBathroom(myId);
		sem_wait(&lock);
		menUsingBathroom--;
		if (menUsingBathroom == 0 && womenWaiting > 0){
			womenWaiting--;
			sem_post(&wEnter);
		}
		sem_post(&lock);
	}		
	return NULL;
}

void * wWork(void * id){
	long myId = (long) id;
	while (true){
		//atomically generate worktime and go to work, as we cannot have concurrent calls to rand
		sem_wait(&lock);
		unsigned int workTime = rand() % 60;
		cout << "worker " << myId << " is boutta work for: " << workTime << " seconds" << endl;
		sem_post(&lock);
		sleep(workTime);
		cout << "worker " << myId << " needs to go to the bathroom!" << endl;
		
		//wait for bathroom to be empty of women
		sem_wait(&lock);
		if (menUsingBathroom > 0){
			womenWaiting++;
			sem_post(&lock);
			sem_wait(&wEnter);
		}
		womenUsingBathroom++;
		//empty cue, pass the baton
		if (womenWaiting > 0){
			womenWaiting--;
			sem_post(&wEnter);
		}
		sem_post(&lock); //why?
		useBathroom(myId);
		sem_wait(&lock);
		womenUsingBathroom--;
		if (womenUsingBathroom == 0 && menWaiting > 0){
			menWaiting--;
			sem_post(&mEnter);
		}
		sem_post(&lock);
	}		
	return NULL;
}

void useBathroom(long id){
	sem_wait(&lock);
	unsigned int bathroomTime = rand() % 10;
	cout << "Worker number " << id << " is using the bathroom for " << bathroomTime << " seconds!" << endl;
	printf("men using bathroom: %d, women using bathroom: %d, men in line: %d, women in line: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
	sem_post(&lock);
	sleep(bathroomTime);
	cout << "worker number " << id << " is leaving the bathroom" << endl;
	return;
}
