#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
pthread_barrier_t pthread_barrier;


void *f(void *arg)
{
	int thread_id = *(int *)arg;
	
	if (thread_id == 1) {
		printf("1\n");
	}

	pthread_barrier_wait(&pthread_barrier);

	if (thread_id == 0) {
		printf("2\n");
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];
	int arguments[NUM_THREADS];

	r =  pthread_barrier_init(&pthread_barrier, NULL, NUM_THREADS);

	if (r) {
		printf("Eroare la crearea barierei \n");
		exit(-1);
	}

	for (i = 0; i < NUM_THREADS; i++) {
		arguments[i] = i;
		r = pthread_create(&threads[i], NULL, f, &arguments[i]);

		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	r = pthread_barrier_destroy(&pthread_barrier);

	if (r) {
		printf("Eroare la distrugerea barierei \n");
		exit(-1);
	}

	return 0;
}
