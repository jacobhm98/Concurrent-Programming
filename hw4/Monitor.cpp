
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include "Monitor.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#define DEBUG 0
using std::cout;
using std::endl;
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
#if DEBUG == 1
		cout << "man number " << id << " has joined the queue" << endl;
#endif
		pthread_cond_wait(&menQueue, &lock);
	}
	menUsingBathroom++;
	if (menWaiting > 0){
		menWaiting--;
		pthread_cond_signal(&menQueue);
	}
#if DEBUG == 1
	cout << "man number " << id << " is entering the bathroom" << endl;
#endif
	pthread_mutex_unlock(&lock);
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
	return;
}
void Monitor::womanEnter(long id){
	pthread_mutex_lock(&lock);
	if (menUsingBathroom > 0 || menWaiting > 0){
		womenWaiting++;
#if DEBUG == 1
		cout << "woman number " << id << " has joined the queue" << endl;
#endif
		pthread_cond_wait(&womenQueue, &lock);	
	}
	womenUsingBathroom++;
	if (womenWaiting > 0){
		womenWaiting--;
		pthread_cond_signal(&womenQueue);
	}
#if DEBUG == 1
	cout << "woman number " << id << " is entering the bathroom" << endl;
#endif
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
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
	cout << "man number " << id << " is leaving the bathroom" << endl;
#endif
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
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
	cout << "woman number " << id << " is leaving the bathroom" << endl;
#endif
	printf("Men in bathroom: %d, Women in bathroom: %d, Men waiting: %d, Women waiting: %d\n", menUsingBathroom, womenUsingBathroom, menWaiting, womenWaiting);
	pthread_mutex_unlock(&lock);
	return;
	
	
}

