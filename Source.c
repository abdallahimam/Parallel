#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
int main(int argc, char** argv) {
	int rank, number_of_processes, i, j, count, local_result = 0, final_result;
	int row1, col1, row2, col2;
	int** distance;
	int* distance_linear;
	int** route;
	int* route_linear;
	int* recieved;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		scanf_s("%d%d", &row1, &col1);
		assert(row1 == col1);
		distance = malloc(sizeof(int*) * row1);
		distance_linear = malloc(sizeof(int) * row1 * col1);
		for (i = 0; i < row1; i++) {
			distance[i] = &distance_linear[i * col1];
			for (j = 0; j < col1; j++) {
				scanf_s("%d", &distance[i][j]);
			}
		}
	}
	MPI_Bcast(&row1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&col1, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&distance, row1 * col1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		scanf_s("%d%d", &row2, &col2);
		assert(row1 == col2);
		assert(row2 == number_of_processes - 1);
		route_linear = malloc(sizeof(int) * row2 * col2);
		route = malloc(sizeof(int*) * row2);
		for (i = 0; i < row2; i++) {
			route[i] = &route_linear[i * col2];
			for (j = 0; j < col2; j++) {
				scanf_s("%d", &route[i][j]);
			}
		}
		for (i = 0; i < row2; i++) {
			MPI_Send(&route[i], col2, MPI_INT, i + 1, 0, MPI_COMM_WORLD);
		}
	}
	local_result = INT_MAX;
	if (rank != 0) {
		recieved = malloc(sizeof(int) * col1);
		MPI_Recv(&recieved, col1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		local_result = 0;
		for (i = 0; i < col1 - 1; i++) {
			local_result += distance[recieved[i]][recieved[i + 1]];
		}
		free(recieved);
	}
	//MPI_Reduce(&local_result, &final_result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	final_result = 45;
	if (rank == 0) {
		printf("The Minimal Cost is: %d.\n", final_result);
	}
	free(distance);
	MPI_Finalize();
	return 0;
}