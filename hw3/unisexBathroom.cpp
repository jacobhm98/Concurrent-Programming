
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>
#define DEBUG 1
#define DEFAULT_SIZE 1000
using namespace std;
using namespace std;

int main(int argc, char **argv){
	int men;
	int women;
	men = (argc > 1) ? atoi(argv[1]) : DEFAULT_SIZE;
	women = (argc > 2) ? atoi(argv[2]) : DEFAULT_SIZE;
	cout << "number of men: " << men << " number of women: " << women << endl;
	return 0;

}
