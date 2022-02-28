#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define min(a,b) (((a)<(b))?(a):(b))

pthread_barrier_t pthread_barrier;
pthread_mutex_t mutex;
struct app_sum{
	int app_on_this_thread;
	int summ_on_this_thread;
};


int *arr;
int size;
int nr_of_threads;
int nr_to_be_found;
struct app_sum *thread_struct;

void *f(void *arg) {
    int thread_id = *(int *)arg;

    int start = thread_id * (double)size / nr_of_threads;
    int end = min((thread_id + 1) * (double)size / nr_of_threads, size);

	for (int i = start; i < end; i++) {
		if (arr[i] == nr_to_be_found) {
			thread_struct[thread_id].app_on_this_thread += 1;
			thread_struct[thread_id].summ_on_this_thread += i;
		}
	}

	pthread_barrier_wait(&pthread_barrier);
    
	if (thread_id == 0) {
		for (int i = 0; i < nr_of_threads; i++) {
			if(i != thread_id) {
				thread_struct[thread_id].app_on_this_thread += thread_struct[i].app_on_this_thread;
				thread_struct[thread_id].summ_on_this_thread += thread_struct[i].summ_on_this_thread;
			}
		}
		printf("%d %d \n", thread_struct[thread_id].app_on_this_thread, thread_struct[thread_id].summ_on_this_thread);
	}

	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    
    size = atoi(argv[1]);
    nr_of_threads = atoi(argv[2]);
	nr_to_be_found = atoi(argv[3]);
	
	thread_struct = malloc(nr_of_threads * sizeof(struct app_sum));
	arr = malloc(size * sizeof(int));
	for(int i = 0; i < size; i++){
		arr[i] = (i + 3) % 5;
	}

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
		thread_struct[id].app_on_this_thread = 0;
		thread_struct[id].summ_on_this_thread = 0;

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
