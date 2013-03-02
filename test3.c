/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
static __inline__ unsigned long long  getticks(void);


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
	
	free(square);
	pthread_exit(0);
}

int main() {
	pthread_t t[10];
	thread_data td[10];
	int i, j;
	struct timespec fa, fb, ta, tb;
	
	/*clock_gettime(CLOCK_REALTIME, &fa);*/
	
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 10; i++)
			td[i].thread_no = i+1;

		for(i = 0; i < 10; i++) {
		unsigned long long start=getticks();		
/*	clock_gettime(CLOCK_REALTIME, &ta);*/
			pthread_create(&t[i], NULL, (void *) &test, (void *) &td[i]);
		/*	clock_gettime(CLOCK_REALTIME, &tb);*/
   		/* __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));*/
		printf("%lld\n",getticks()-start);
		}
		for(i = 0; i < 10; i++)
			pthread_join(t[i], NULL);
	}

	/*clock_gettime(CLOCK_REALTIME, &fb);
	printf("%.6f\n", (double) fb.tv_nsec - fa.tv_nsec);
*/
	exit(0);
}
static __inline__ unsigned long long getticks(void)
{
     unsigned a, d;
     asm volatile("cpuid"); 
      asm volatile("rdtsc" : "=a" (a), "=d" (d));
    unsigned long long ret= (((unsigned long long)a) | (((unsigned long long)d) << 32));
	return ret;
}


