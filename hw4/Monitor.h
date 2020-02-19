#ifndef _REENTRANT
#define _REENTRANT
#endif

#ifndef MONITOR_H
#define MONITOR_H

class Monitor{

	public:
		void manEnter();
		void womanEnter();
		void manExit();
		void womanExit();

};

#endif

