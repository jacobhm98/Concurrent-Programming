#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int threadcount;

void* Hello(void* rank);

int main(int argc, char* argv[]){
	long thread;
	pthread_t* thread_handles;

	threadcount = strtol(argv[1], NULL, 10);
	thread_handles = malloc(threadcount * sizeof(pthread_t));

	for (thread = 0; thread < threadcount; thread++){
		pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
	}
	printf("hello from the main thread\n");

	for (thread = 0; thread < threadcount; thread++){
		pthread_join(thread_handles[thread], NULL);
	}
	free(thread_handles);
	return 0;
}

void* Hello(void* rank){
	long myRank = (long) rank;
	printf("Hello from thread %ld of %d\n", myRank, threadcount);
	return NULL;
}
