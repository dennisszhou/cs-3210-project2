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

static __inline__ unsigned long long rdtsc(void)
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

void test(void *ptr) {
	thread_data *data;
	data = (thread_data *) ptr;
	int i;	
	int *square = malloc(100 * sizeof(int));

	for(i = 0; i < 100; i++)
		square[i] = i+1;
	
	for(i = 0; i < 100; i++)
		square[i] *= square[i];
	
	free(square);
	pthread_exit(0);
}

int main() {
	pthread_t t[10];
	thread_data td[10];
	int i, j;
	struct timespec fa, fb, ta, tb;
	
	clock_gettime(CLOCK_REALTIME, &fa);
	
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 10; i++)
			td[i].thread_no = i+1;

		for(i = 0; i < 10; i++) {
			clock_gettime(CLOCK_REALTIME, &ta);
			pthread_create(&t[i], NULL, (void *) &test, (void *) &td[i]);
			clock_gettime(CLOCK_REALTIME, &tb);
			printf("%.6f\n", (double) tb.tv_nsec - ta.tv_nsec);
		}
		for(i = 0; i < 10; i++)
			pthread_join(t[i], NULL);
	}

	clock_gettime(CLOCK_REALTIME, &fb);
	printf("%.6f\n", (double) fb.tv_nsec - fa.tv_nsec);
	printf("%llu\n", rdtsc());

	exit(0);
}
