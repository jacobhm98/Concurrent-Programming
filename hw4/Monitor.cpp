
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
}

void Monitor::manEnter(){
	
}
void Monitor::womanEnter(){
	
}
	
void Monitor::manExit(){
	
}
void Monitor::womanExit(){
	
	
}

