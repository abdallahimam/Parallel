#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROOT 0
#define TAG_1 1
#define TAG_2 2

int main(int args, char ** argvs) {
	double start_time, end_time;
	int rank, size, number_of_processes, i, j, number, sub_range, reminder, start, count, recieved;
	long long int result = 1;
	MPI_Status status;
	MPI_Init(&args, &argvs);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();
	number_of_processes = size - 1;
	if (rank == ROOT) {
		scanf_s("%d", &number);
		start_time = MPI_Wtime();
		sub_range = number / number_of_processes;
		reminder = number % number_of_processes;
		start = 1;
		for (i = 0; i < number_of_processes; i++) {
			count = sub_range;
			if (i < reminder) {
				count++;
			}
			MPI_Send(&count, 1, MPI_INT, i + 1, TAG_1, MPI_COMM_WORLD);
			MPI_Send(&start, 1, MPI_INT, i + 1, TAG_2, MPI_COMM_WORLD);
			start += count;
		}
		start = 1;
		long long int tempArr[100];
		for (i = 0; i < number_of_processes; i++) {
			count = sub_range;
			long long int temp;
			MPI_Recv(&temp, 1, MPI_LONG_LONG_INT, i + 1, TAG_1, MPI_COMM_WORLD, &status);
			if (i < reminder) {
				count++;
			}
			/*printf("p%d: calculate partial factorial from %d to %d = %d",
				(i + 1), start, (start + count - 1), start);
			int j;
			for (j = start + 1; j < count + start; j++) {
				printf("*%d", j);
			}
			printf(" = %lld.\n", temp);*/
			result *= temp;
			start += count;
			tempArr[i] = temp;
		}
		/*printf("P0 will have factorial(%d) = ", number);
		for (i = 0; i < number_of_processes - 1; i++) {
			printf("%d*", tempArr[i]);
		}
		printf("%d = ", tempArr[number_of_processes - 1]);*/
		printf("%lld.\n", result);
	}
	else {
		MPI_Recv(&count, 1, MPI_INT, ROOT, TAG_1, MPI_COMM_WORLD, &status);
		MPI_Recv(&recieved, 1, MPI_INT, ROOT, TAG_2, MPI_COMM_WORLD, &status);
		for (i = recieved; i < recieved + count; i++) {
			result *= i;
		}
		MPI_Send(&result, 1, MPI_LONG_LONG_INT, ROOT, TAG_1, MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	end_time = MPI_Wtime();
	MPI_Finalize();
	if (rank == ROOT) {
		printf("Elapsed Time for Bcast and Reduce: %f seconds.\n", end_time - start_time);
	}
	return 0;
}