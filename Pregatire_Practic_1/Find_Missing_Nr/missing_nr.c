#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define min(a,b) (((a)<(b))?(a):(b))


pthread_barrier_t pthread_barrier;
pthread_mutex_t mutex;

int *arr;
int array_size;
int nr_of_threads;
int gauss_sum;
int sum;

void *f(void *arg) {
    int thread_id = *(int *)arg;

    int start = thread_id * (double)array_size / nr_of_threads;
    int end = min((thread_id + 1) * (double)array_size / nr_of_threads, array_size);

    for (int i = start; i < end; i++) {
        pthread_mutex_lock(&mutex);
        sum += arr[i];
        pthread_mutex_unlock(&mutex);
    }

    pthread_barrier_wait(&pthread_barrier);

    if (thread_id == 0) {
        printf("Missing nr is %d. \n", (gauss_sum - sum));
    }

	pthread_exit(NULL);
}



int main(int argc, char *argv[]) {

    array_size = atoi(argv[1]);
    gauss_sum = array_size * (array_size  + 1) / 2;
    nr_of_threads = atoi(argv[2]);

    arr = malloc(array_size - 1 * sizeof(int));

    for (int i = 0; i < array_size - 1; i++) {
        scanf("%d", &arr[i]);
    }
    printf("\n");

	pthread_t threads[nr_of_threads];
  	int r;
    int thread_id[nr_of_threads];

    r =  pthread_barrier_init(&pthread_barrier, NULL, nr_of_threads);
	if (r) {
		printf("Eroare la crearea barierei \n");
		exit(-1);
	}

    r = pthread_mutex_init(&mutex, NULL);

	if (r) {
		printf("Eroare la initializarea mutex-ului \n");
		exit(-1);
	}

  	for (int id = 0; id < nr_of_threads; id++) {

        thread_id[id] = id;

		r = pthread_create(&threads[id], NULL, f, &thread_id[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %d\n", id);
	  		exit(-1);
		}
        
  	}

  	for (int id = 0; id < nr_of_threads; id++) {
		r = pthread_join(threads[id], NULL);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %d\n", id);
	  		exit(-1);
		}
  	}

    r = pthread_barrier_destroy(&pthread_barrier);

	if (r) {
		printf("Eroare la distrugerea barierei \n");
		exit(-1);
	}

    r = pthread_mutex_destroy(&mutex);

	if (r) {
		printf("Eroare la distrugerea mutex-ului \n");
		exit(-1);
	}

  	pthread_exit(NULL);
}
