#ifndef _REENTRANT
#define _REENTRANT
#endif

#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>
#endif

class Monitor{
	//counters, synchronization structures, and timer
	int womenUsingBathroom;
	int menUsingBathroom;
	int womenWaiting;
	int menWaiting;
	pthread_mutex_t lock;
	pthread_cond_t menQueue;
	pthread_cond_t womenQueue;	
	double startTime;
	public:
		//constructor exit/enter controls for each gender and timing logic
		Monitor();
		void manEnter(long);
		void womanEnter(long);
		void manExit(long);
		void womanExit(long);
		double read_timer();
		double timePassed();

};


