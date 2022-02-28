#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define min(a,b) (((a)<(b))?(a):(b))


pthread_barrier_t pthread_barrier;

int **graph;
int graph_size;
int nr_of_threads;

void *f(void *arg) {
    int thread_id = *(int *)arg;

    int start = thread_id * (double)graph_size / nr_of_threads;
    int end = min((thread_id + 1) * (double)graph_size / nr_of_threads, graph_size);
    
    for (int k = 0; k < graph_size; k++) {
        for (int i = start; i < end; i++) {
            for (int j = 0; j < graph_size; j++) {
                graph[i][j] = min(graph[i][k] + graph[k][j], graph[i][j]);
            }
        }
        pthread_barrier_wait(&pthread_barrier);
    }

    if (thread_id == 0) {
        for (int i = 0; i < graph_size; i++) {
            for (int j = 0; j < graph_size; j++) {
                printf("%d ", graph[i][j]);
            }
            printf("\n");
        }
    }

	pthread_exit(NULL);
}



int main(int argc, char *argv[]) {

   
    graph_size = atoi(argv[1]);
    nr_of_threads = atoi(argv[2]);

    graph = calloc(graph_size, sizeof(int *));
    for(int i = 0; i < graph_size; i++) {
        graph[i] = calloc(graph_size, sizeof(int));
	}

    for (int i = 0; i < graph_size; i++) {
        for (int j =0; j < graph_size; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

	pthread_t threads[nr_of_threads];
  	int r;
    int thread_id[nr_of_threads];

    r =  pthread_barrier_init(&pthread_barrier, NULL, nr_of_threads);
	if (r) {
		printf("Eroare la crearea barierei \n");
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

  	pthread_exit(NULL);
}
