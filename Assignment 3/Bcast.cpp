#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROOT 0

int main(int args, char ** argvs) {
	double start_time, end_time;
	int rank, size, number_of_processes, i, number, start, end;
	long long int result = 1;
	long long int total_result = 1;
	MPI_Init(&args, &argvs);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();
	number_of_processes = size - 1;
	if (rank == ROOT) {
		scanf_s("%d", &number);
		start_time = MPI_Wtime();
	}
	MPI_Bcast(&number, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
	if (rank != ROOT) {
		start = (rank - 1) * (number / number_of_processes) + 1;
		end = rank * (number / number_of_processes);
		if (rank == number_of_processes) {
			end = number;
		}
		/*printf("p%d: calculate partial factorial from %d to %d = %d",
			rank, start, end, start);*/
		result = start;
		for (i = start + 1; i <= end; i++) {
			result *= i;
			//printf("*%d", i);
		}
		//printf(" = %lld.\n", result);
	}
	MPI_Reduce(&result, &total_result, 1, MPI_LONG_LONG_INT, MPI_PROD, ROOT, MPI_COMM_WORLD);
	if (rank == ROOT) {
		printf("After reduction, P0 will have factorial(%d) = ", number);
		printf("%lld.\n", total_result);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	end_time = MPI_Wtime();
	MPI_Finalize();
	if (rank == ROOT) {
		printf("Elapsed Time for Bcast and Reduce: %f seconds.\n", end_time - start_time);
	}
	return 0;
}