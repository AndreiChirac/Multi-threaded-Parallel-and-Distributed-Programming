#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MASTER 0

int main (int argc, char *argv[]) {

    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    MPI_Status status;
    int value = rank * 2;
    int receivedValue;

    for (int i = 0; i < numtasks; i++) {
        if (i % 2 == 0) {
        
            if( rank % 2 == 0) {
                
                if (rank != numtasks - 1 ) {

                    MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

                    MPI_Recv(&receivedValue, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);

                    if (receivedValue > value) {
                        value = receivedValue;
                    }
                }

            } else {
                
                MPI_Recv(&receivedValue, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

                MPI_Send(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

                if (receivedValue < value) {
                    value = receivedValue;
                }
            }

            
            
        } else {
                
            if( rank % 2 == 0) {
                
                if (rank != 0) {
                    MPI_Send(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

                    MPI_Recv(&receivedValue, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

                    if (receivedValue < value) {
                        value = receivedValue;
                    }
                }

            } else {
                
                if (rank != numtasks - 1) {
                    
                    MPI_Recv(&receivedValue, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);

                    MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

                    if (receivedValue > value) {
                        value = receivedValue;
                    }
                }
            }
            
        }
   
    }
    
    printf("%d - %d \n", rank, value);
    
    MPI_Finalize();

}