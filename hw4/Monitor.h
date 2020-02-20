#ifndef _REENTRANT
#define _REENTRANT
#endif

#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>
#endif

class Monitor{
	int womenUsingBathroom;
	int menUsingBathroom;
	int womenWaiting;
	int menWaiting;
	pthread_mutex_t lock;
	pthread_cond_t menQueue;
	pthread_cond_t womenQueue;	
	public:
		Monitor();
		void manEnter(long);
		void womanEnter(long);
		void manExit(long);
		void womanExit(long);

};


