/*
 * The implementation of the monitor interface specified in Monitor.h. It implements the waiting logic of the last solution 
 * using two condition variables in place of the signalling semaphores, and a mutex lock in place of the binary semaphore. Helper class to unisexBathroom.cpp.
 *
 * Two different types of printouts available, define DEBUG to either 0 or 1
 */
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include "Monitor.h"	//interface specification
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/time.h>
#define DEBUG 1
using std::cout;
using std::endl;

//timer
double Monitor::read_timer() {
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

//return time program has been running
double Monitor::timePassed(){
	auto currentTime = read_timer();
	return currentTime - startTime;
	
}

//initialize locks and condvariables, counters, and start the timer.
Monitor::Monitor(){
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&menQueue, NULL);
	pthread_cond_init(&womenQueue, NULL);
	menUsingBathroom = 0;
	womenUsingBathroom = 0;
	menWaiting = 0;
	womenWaiting = 0;
	startTime = read_timer();
}


void Monitor::manEnter(long id){
	pthread_mutex_lock(&lock);
	if (womenUsingBathroom > 0 || womenWaiting > 0){
		menWaiting++;
#if DEBUG == 1
		cout << "man number " << id << " has joined the queue! Timestamp: " << timePassed() << endl;
#endif
		pthread_cond_wait(&menQueue, &lock);
	}
	menUsingBathroom++;
	if (menWaiting > 0){
		menWaiting--;
		pthread_cond_signal(&menQueue);
	}
#if DEBUG == 1
	cout << "man number " << id << " is entering the bathroom! Timestamp: " << timePassed()  << endl;
#endif
	pthread_mutex_unlock(&lock);
#if DEBUG == 0
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
#endif
	return;
}
void Monitor::womanEnter(long id){
	pthread_mutex_lock(&lock);
	if (menUsingBathroom > 0 || menWaiting > 0){
		womenWaiting++;
#if DEBUG == 1
		cout << "woman number " << id << " has joined the queue! Timestamp: " << timePassed() << endl;
#endif
		pthread_cond_wait(&womenQueue, &lock);	
	}
	womenUsingBathroom++;
	if (womenWaiting > 0){
		womenWaiting--;
		pthread_cond_signal(&womenQueue);
	}
#if DEBUG == 1
	cout << "woman number " << id << " is entering the bathroom! Timestamp: " << timePassed() << endl;
#endif
#if DEBUG == 0
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
#endif
	pthread_mutex_unlock(&lock);
	return;
	
}
	
void Monitor::manExit(long id) {
	pthread_mutex_lock(&lock);
	menUsingBathroom--;
	if (menUsingBathroom == 0 && womenWaiting > 0){
		womenWaiting--;
		pthread_cond_signal(&womenQueue);
	}
#if DEBUG == 1
	cout << "man number " << id << " is leaving the bathroom! Timestamp: " << timePassed() << endl;
#endif
#if DEBUG == 0
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
#endif
	pthread_mutex_unlock(&lock);
	return;
}
void Monitor::womanExit(long id){
	pthread_mutex_lock(&lock);
	womenUsingBathroom--;
	if (womenUsingBathroom == 0 && menWaiting > 0){
		menWaiting--;
		pthread_cond_signal(&menQueue);
	}
#if DEBUG == 1
	cout << "woman number " << id << " is leaving the bathroom! Timestamp: " << timePassed() << endl;
#endif
#if DEBUG == 0
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
#endif
	pthread_mutex_unlock(&lock);
	return;
	
	
}

