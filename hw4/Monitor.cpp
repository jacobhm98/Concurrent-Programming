
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include "Monitor.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

Monitor::Monitor(){

	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&menQueue, NULL);
	pthread_cond_init(&womenQueue, NULL);
	menUsingBathroom = 0;
	womenUsingBathroom = 0;
	menWaiting = 0;
	womenWaiting = 0;
}

void Monitor::manEnter(long id){

	pthread_mutex_lock(&lock);
	if (womenUsingBathroom > 0 || womenWaiting > 0){
		menWaiting++;
		pthread_cond_wait(&menQueue, &lock);	
	}
	menUsingBathroom++;
	if (menWaiting > 0){
		menWaiting--;
		pthread_cond_signal(&menQueue);
	}
	pthread_mutex_unlock(&lock);
	return;
}
void Monitor::womanEnter(long id){
	pthread_mutex_lock(&lock);
	if (menUsingBathroom > 0 || menWaiting > 0){
		menWaiting++;
		pthread_cond_wait(&womenQueue, &lock);	
	}
	womenUsingBathroom++;
	if (womenWaiting > 0){
		womenWaiting--;
		pthread_cond_signal(&womenQueue);
	}
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
	pthread_mutex_unlock(&lock);
	return;
	
	
}

