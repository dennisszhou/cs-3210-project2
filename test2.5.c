/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <sys/time.h>
#include <time.h>
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */

typedef struct t_data {
  int thread_no;
} thread_data;

void test(void *ptr) {
	thread_data *data;
	data = (thread_data *) ptr;
	int i;	
	int *square = malloc(100 * sizeof(int));

	for(i = 0; i < 100; i++)
		square[i] = i+1;
	
	for(i = 0; i < 100; i++)
		square[i] *= square[i];

	printf("Thread %d\n", data->thread_no);
	
	free(square);
	pthread_exit(0);
}

int main() {
	pthread_t t[10];
	thread_data td[10];
	int i, j;
	
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 10; i++)
			td[i].thread_no = i+1;

		for(i = 0; i < 10; i++)
			pthread_create(&t[i], NULL, (void *) &test, (void *) &td[i]);

		for(i = 0; i < 10; i++)
			pthread_join(t[i], NULL);
	}
	
	exit(0);
}
