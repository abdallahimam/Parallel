#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
int main(int argc, char** argv)
{
    int r1, c1, r2, c2, r3, c3;
    int** mat1;
    int** mat2;
    int** mat3;
    int*recieve1;
    int*recieve2;

    int rank, size, remainder;
    MPI_Status status;
    MPI_Comm world;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
    	printf("Enter dimension of first matrix: ");
    	scanf("%d%d", &r1, &c1);
    	mat1 = malloc(r1 * sizeof(int*));
    	for (int i = 0; i < r1; i++) {
    		mat1[i] = malloc(r1 * c1 sizeof(int));
    		for (int j = 0; j < c1; j++) {
    			scanf("%d",&mat1[i][j]);
    		}
    	}
    	printf("Enter dimension of second matrix: ");
    	scanf("%d%d", &r2, &c2);
    	if(c1 == r2) {
    		r3 = r1;
    		c3 = c2;
    	} else if (r1 == c2) {
    		r3 = r2;
    		c3 = c2;
    	} else {
    		
    	}
    	while(c1 != r2) {
    		printf("Invalid dimesion!\nEnter dimension of second matrix: ");
    		scanf("%d%d", &r2, &c2);
    	}
    	mat2 = malloc(r2 * sizeof(int*));
    	for (int i = 0; i < r2; i++) {
    		mat2[i] = malloc(r2 * c2 sizeof(int));
    		for (int j = 0; j < c2; j++) {
    			scanf("%d",&mat2[i][j]);
    		}
    	}
    }
    MPI_Bcast();
    MPI_Finalize();
    printf("Hello world!\n");
    return 0;
}
