#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a,b) (((a)<(b))?(a):(b))

pthread_barrier_t pthread_barrier;
pthread_mutex_t mutex;
struct min_max{
	int max;
	int min;
};

int size;
int nr_of_threads;
int *arr;
struct min_max *thread_struct;

void *f(void *arg) {
    int thread_id = *(int*)arg;

    int start = thread_id * (double)size / nr_of_threads;
    int end = min((thread_id + 1) * (double)size / nr_of_threads, size);

	for (int i = start; i < end; i++) {
		if(arr[i] > thread_struct[thread_id].max){
			thread_struct[thread_id].max = arr[i];
		}

		if(arr[i] < thread_struct[thread_id].min){
			thread_struct[thread_id].min = arr[i];
		}
	}

	pthread_barrier_wait(&pthread_barrier);

    //pthread_mutex_lock(&mutex);
    //pthread_mutex_unlock(&mutex);
	if(thread_id == 0) {
		int min = 9999;
		int max = -1;
		for (int i = 0; i < nr_of_threads; i++) {
			
			if(thread_struct[i].max > max){
				max = thread_struct[i].max;
			}	

			if(thread_struct[i].min < min){
				min = thread_struct[i].min;
			}
		}
		printf("Minimul este : %d si maximul este %d \n", min, max);
	}
   
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    
    size = atoi(argv[1]);
    nr_of_threads = atoi(argv[2]);

	thread_struct = malloc(nr_of_threads * sizeof(struct min_max));
	arr = calloc(size, sizeof(int));
	for (int i = 0; i < size; i++){
		scanf("%d", &arr[i]);
	}

	printf("Vrem sa gasim minimul si maximul din: \n");
	for (int i = 0; i < size; i++){
		printf("%d ", arr[i]);
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

		thread_struct[id].max = -1;
		thread_struct[id].min = 99999;

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
