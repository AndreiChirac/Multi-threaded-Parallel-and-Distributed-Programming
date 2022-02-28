#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZE 500000
 
int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // The current process ID / Rank.

    srand(42);
    int num1[SIZE], num2[SIZE];
    MPI_Status status;

    if (rank == 0) {
        for (int i = 0; i < SIZE; i++) {
            num1[i] = 100;
        }
        // MPI_Send(&num1, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
        // MPI_Recv(&num2, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // declarăm mărimea buffer-ului
        int buffer_attached_size = MPI_BSEND_OVERHEAD + SIZE * sizeof(int);
	    // se creează spațiul folosit pentru buffer
        char* buffer_attached = malloc(buffer_attached_size);
	    // se creează buffer-ul MPI folosit pentru trimiterea mesajelor
        MPI_Buffer_attach(buffer_attached, buffer_attached_size);
 
        MPI_Bsend(&num1, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&num2, SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

    } else {
        for (int i = 0; i < SIZE; i++) {
            num2[i] = 200;
        }
        // MPI_Send(&num2, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // MPI_Recv(&num1, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // declarăm mărimea buffer-ului
        int buffer_attached_size = MPI_BSEND_OVERHEAD + SIZE * sizeof(int);
	    // se creează spațiul folosit pentru buffer
        char* buffer_attached = malloc(buffer_attached_size);
	    // se creează buffer-ul MPI folosit pentru trimiterea mesajelor
        MPI_Buffer_attach(buffer_attached, buffer_attached_size);
 
        MPI_Bsend(&num2, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Recv(&num1, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();
 
}