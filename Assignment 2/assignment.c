#include "stdafx.h"
#include <mpi.h>
#include <stdlib.h>
#define MASTER 0
#define MAX_ARRAY_ELEMENTS 100
#define tag1 1
#define tag2 2
int main(int args, char **argvs)
{
	int rank, array_size, i, number_element_send, reminder, offset, max, j, commulative_offset,
		number_element_send_plus, number_of_slaves, number_element_recieved, number_of_processes;
	int array[MAX_ARRAY_ELEMENTS];
	int array2[MAX_ARRAY_ELEMENTS];
	MPI_Status status;
	MPI_Init(&args, &argvs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
	if (rank == MASTER) {
		number_of_slaves = number_of_processes - 1;
		printf("Hello from master process.\n");
		printf("Number of slave processes is: %d\n", number_of_slaves);
		printf("Please enter size of array: \n");
		scanf_s("%d", &array_size);
		printf("Please enter array elements: \n");
		for (i = 0; i < array_size; i++) {
			scanf_s("%d", &array[i]);
		}
		number_element_send = array_size / number_of_slaves;
		number_element_send_plus = number_element_send + 1;
		reminder = array_size % number_of_slaves;
		offset = 0;
		for (i = 1; i <= reminder; i++) {
			MPI_Send(&number_element_send_plus, 1, MPI_INT, i, tag1, MPI_COMM_WORLD);
			MPI_Send(&array[offset], number_element_send_plus, MPI_INT, i, tag2, MPI_COMM_WORLD);
			offset += number_element_send_plus;
		}
		for (i = reminder + 1; i < number_of_processes; i++) {
			MPI_Send(&number_element_send, 1, MPI_INT, i, tag1, MPI_COMM_WORLD);
			MPI_Send(&array[offset], number_element_send, MPI_INT, i, tag2, MPI_COMM_WORLD);
			offset += number_element_send;
		}
		int max, maxOffset, tempMax;
		MPI_Recv(&offset, 1, MPI_INT, 1, tag1, MPI_COMM_WORLD, &status);
		MPI_Recv(&tempMax, 1, MPI_INT, 1, tag2, MPI_COMM_WORLD, &status);
		maxOffset = offset;
		max = tempMax;
		commulative_offset = 0;
		printf("Hello from slave %d Max number in my portion is %d and index is %d.\n", status.MPI_SOURCE, tempMax, offset);
		for (i = 2; i < number_of_processes; i++) {
			MPI_Recv(&offset, 1, MPI_INT, i, tag1, MPI_COMM_WORLD, &status);
			MPI_Recv(&tempMax, 1, MPI_INT, i, tag2, MPI_COMM_WORLD, &status);
			printf("Hello from slave %d Max number in my portion is %d and index is %d.\n", status.MPI_SOURCE, tempMax, offset);
			if (i <= reminder) {
				commulative_offset += number_element_send_plus;
			}
			else {
				commulative_offset += number_element_send;
			}
			if (tempMax > max) {
				max = tempMax;
				maxOffset = commulative_offset + offset;
			}
		}
		printf("Master process announces: max number is %d and its index is %d.", max, maxOffset);
	}
	else {
		MPI_Recv(&number_element_recieved, 1, MPI_INT, MASTER, tag1, MPI_COMM_WORLD, &status);
		MPI_Recv(&array2, number_element_recieved, MPI_INT, MASTER, tag2, MPI_COMM_WORLD, &status);
		max = array2[0];
		offset = 0;
		for (j = 1; j < number_element_recieved; j++) {
			if (array2[j] > max) {
				max = array2[j];
				offset = j;
			}
		}
		MPI_Send(&offset, 1, MPI_INT, MASTER, tag1, MPI_COMM_WORLD);
		MPI_Send(&max, 1, MPI_INT, MASTER, tag2, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}

