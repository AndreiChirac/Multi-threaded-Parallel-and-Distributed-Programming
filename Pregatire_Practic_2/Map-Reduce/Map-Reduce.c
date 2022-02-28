#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MASTER 0

int main (int argc, char *argv[]) {

    int  numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    if (numtasks == 6) {
        
        int N;
        char **file;
        int start;
        int end;
        
        MPI_Status status;
        int vow = 0;
        int cons = 0;
        
        if (rank == MASTER) {
            //printf("sadasd %s ", argv[1]);

            FILE *in_file = fopen(argv[1], "r");;
            
            if (in_file == NULL) {   
                printf("Error! Could not open file\n"); 
                exit(-1);
            } 
            
            fscanf(in_file, "%d", &N);

            file = (char **)malloc(N * sizeof(char *));
            for (int i = 0; i < N; i++) {
                file[i] = (char *)malloc(15 * sizeof(char));
                fscanf(in_file, "%s", file[i]);
            }

            for (int i = 1; i <= 3; i++) {

                start = (i - 1) * (double)N / 3;
                end = MIN((i) * (double)N / 3, N);

                int vec_dim = (end - start);
                MPI_Send(&vec_dim, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                for (int j = start; j < end; j++) {
                    MPI_Send(file[j], 15, MPI_CHAR, i, 0, MPI_COMM_WORLD);        
                }
            
            }
            
        } else if (rank == 1 || rank == 2 || rank == 3) {

            MPI_Recv(&N, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);

            file = (char **)malloc(N * sizeof(char *));
            for (int i = 0; i < N; i++) {
                file[i] = (char *)malloc(15 * sizeof(char));
                MPI_Recv(file[i], 15, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD, &status);
                printf("[%d] %s\n", rank, file[i]);

                for (int j = 0; j < 15; j++) {
                    if ((file[i][j] >= 'a' && file[i][j] <= 'z') || (file[i][j] >= 'A' && file[i][j] <= 'Z')) {
                        if(file[i][j] == 'a' || file[i][j] == 'e' || file[i][j] == 'i' || file[i][j] == 'o' || file[i][j] == 'u' ||
                        file[i][j] == 'A' || file[i][j] == 'E' || file[i][j] == 'I' || file[i][j] == 'O' || file[i][j] == 'U')  {
                            vow++;
                        } else {
                            cons++;
                        }
                    } else {
                        break;
                    }
                }
            }

            printf("[%d] v=%d c=%d\n", rank, vow, cons);

            MPI_Send(&vow, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
            MPI_Send(&cons, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);

        } else if (rank == 4){
            int part_vow = 0;

            for (int i = 1; i <= 3; i++) {
                MPI_Recv(&part_vow, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                vow+= part_vow;
            }

            printf("[%d] Vowels: %d \n", rank, vow);
        } else if (rank == 5){
            int part_cons = 0;

            for (int i = 1; i <= 3; i++) {
                MPI_Recv(&part_cons, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                cons+= part_cons;
            }

            printf("[%d] Consonants: %d \n", rank, cons);
        }
    }

    MPI_Finalize();

}