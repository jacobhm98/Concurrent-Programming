#ifndef _REENTRANT
#define _REENTRANT
#endif

#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>

class Monitor{
	int womenUsingBathroom;
	int menUsingBathroom;
	pthread_mutex_t lock;
	pthread_cond_t menQueue;
	pthread_cond_t womenQueue;	
	public:
		Monitor();
		void manEnter();
		void womanEnter();
		void manExit();
		void womanExit();

};

#endif

