#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define ROOT 0

int main (int argc, char *argv[])
{
    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    // Checks the number of processes allowed.
    if (numtasks != 2) {
        printf("Wrong number of processes. Only 2 allowed!\n");
        MPI_Finalize();
        return 0;
    }

    // How many numbers will be sent.
    int send_numbers = 10;
    int rand_nr;
    int rand_tag;
    MPI_Status status;

    if (rank == 0) {
        
        for (int i = 0; i < send_numbers; i++) {
            rand_nr = rand() % 100;
            rand_tag = rand () % 100;
            MPI_Send(&rand_nr, 1, MPI_INT, 1, rand_tag, MPI_COMM_WORLD);
        }
        // Generate the random numbers.
        // Generate the random tags.
        // Sends the numbers with the tags to the second process.
    } else {
        for (int i = 0; i < send_numbers; i++) {
            MPI_Recv(&rand_nr, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Nr: %d, tag %d \n", rand_nr, status.MPI_TAG);
        }
        // Receives the information from the first process.
        // Prints the numbers with their corresponding tags.
    }

    MPI_Finalize();

}

