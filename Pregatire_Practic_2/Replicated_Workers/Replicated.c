#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MASTER 0

int main (int argc, char *argv[]) {

    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int sleepTime;
    int freeWorker;
    int nrTasks;

    MPI_Status status;
    MPI_Request request;
    int flag;

    if (rank == MASTER) { 

        int *workers = (int *)calloc(numtasks, sizeof(int));
        workers[0] = 1;
        int freeWorkers = numtasks - 1;

        int *tasks;
        int nrOfActiveTasks = 0;

        for (size_t z = 0; z < 12; z++) {
          
            freeWorker = -1;

            sleepTime = rand() % 10;
            if (freeWorkers == 0) {
                
                nrOfActiveTasks++;
                if (nrOfActiveTasks == 1) {
                    tasks = (int *)calloc(nrOfActiveTasks, sizeof(int));
                    tasks[nrOfActiveTasks - 1] = sleepTime;
                } else {
                    tasks = (int *)realloc(tasks, nrOfActiveTasks * sizeof(int));
                    tasks[nrOfActiveTasks - 1] = sleepTime;
                }

            } else {

                if (nrOfActiveTasks > 0) {
                    
                    nrOfActiveTasks++;
                    tasks = (int *)realloc(tasks, nrOfActiveTasks * sizeof(int));
                    tasks[nrOfActiveTasks - 1] = sleepTime;
                    
                } 

                for (int i = 0; i < numtasks; i++) {
                    if (workers[i] == 0) {

                        workers[i] = 1;
                        freeWorkers--;
                        printf("%d %d %d \n", nrOfActiveTasks, rank, sleepTime);

                        if(nrOfActiveTasks > 0) {
                            MPI_Send(&tasks[0], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                            printf("%d a trimis \n", rank);
                            nrOfActiveTasks--;
                        } else {
                            MPI_Send(&sleepTime, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                            printf("%d a trimis \n", rank);
                        }

                        break;
                    }
                }
            }
            
            MPI_Irecv(&freeWorker, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &request);
            MPI_Test(&request, &flag, &status);


            MPI_Wait(&request, &status);

            if(freeWorker != -1) {
                workers[freeWorker] = 0;
                freeWorkers++;
            }
            
            
        } 

    } else {
        while (1) {
            MPI_Recv(&sleepTime, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            printf("%d %d \n", rank, sleepTime);
            sleep(sleepTime);

            freeWorker = rank;
            MPI_Isend(&freeWorker, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
            printf("%d a trimis \n", rank);
        }
    }

    MPI_Finalize();

}