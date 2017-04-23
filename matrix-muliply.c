
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 

int *allocate_array(int row_dim, int col_dim)
{
	int *result;
	int i;
	result = (int*) malloc(row_dim * col_dim * sizeof(int));
	return result;
}

int main(int argc, char **argv)
{
	int *mat1, *mat2;
	int r1, c1, r2, c2, r3, c3;
	int i, j, k, num_prs;
	int *apart, *cpart;
	int rank, size, s, ierr, root = 0;
	MPI_Comm world = MPI_COMM_WORLD;
	MPI_Status status;
	ierr = MPI_Init(&argc, &argv);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == root) {
		printf("Enter rows of mat1, cols of mat1 , rows of mat2, cols of mat2:\n");
		scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
		while (c1 != r2) {
		printf("tryAgain\nEnter rows of mat1, cols of mat1 , rows of mat2, cols of mat2:\n");
			scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
		}
		mat1 = allocate_array(r1, c1);
		for (i = 0; i<r1; i++)
			for (j = 0;j<c1; j++)
				scanf("%d", &mat1[i * c1 + j]);
		int scatter_rows = r1 / size;
		scatter_rows *= size;
		num_prs = r1 - scatter_rows;
		int start = scatter_rows * c1;
		for (i = 1; i <= num_prs; i++) {
			MPI_Send(&mat1[start], c1, MPI_INT, i, 0, world);
			start+=c1;
		}
	}
	ierr = MPI_Bcast(&num_prs, 1, MPI_INT, root, MPI_COMM_WORLD);
	ierr = MPI_Bcast(&r1, 1, MPI_INT, root, MPI_COMM_WORLD);
	ierr = MPI_Bcast(&c1, 1, MPI_INT, root, MPI_COMM_WORLD);
	ierr = MPI_Bcast(&r2, 1, MPI_INT, root, MPI_COMM_WORLD);
	ierr = MPI_Bcast(&c2, 1, MPI_INT, root, MPI_COMM_WORLD);
	r3 = r1, c3 = c2;
	apart = allocate_array(r1 / size, c1);
	cpart = allocate_array(r3 / size, c3);
	mat2 = allocate_array(r2, c2);
	ierr = MPI_Scatter(mat1, (r1 / size) * c1, MPI_INT, apart, (r1 / size) * c1, MPI_INT, root,
	MPI_COMM_WORLD);
	if (rank == root) {
		for (i = 0; i<r2; i++)
			for (j = 0;j<c2; j++)
				scanf("%d", &mat2[i * c2 + j]);
	}
	ierr = MPI_Bcast(mat2, r2*c2, MPI_INT, root, MPI_COMM_WORLD);
	for (i = 0; i<r3 / size; i++) {
		for (k = 0; k<c2; k++){
			int sum = 0;
			for (j = 0; j<r2; j++){
				sum += apart[i * c3 + j] * mat2[j * c2 + k];
			}
			cpart[i * c3 + k] = sum;
		}
	}
	int* mat3 = NULL;
	if (rank == root) {
		mat3 = allocate_array(r3, c3);
	}
	ierr = MPI_Gather(cpart, (r3 / size)*c3, MPI_INT, mat3, (r3 / size)*c3, MPI_INT, root, MPI_COMM_WORLD);
	
	if (rank > 0 && rank <= num_prs) {
		int* rec  = (int*)malloc(c1 * sizeof(int));
		int* send = (int*)malloc(c3 * sizeof(int));
		MPI_Recv(rec, c1, MPI_INT, 0, 0, world, &status);
		for (i = 0; i<c2; i++) {
			int sum  = 0, rr = 0;
			for (j = 0; j<r2; j++){
				sum += rec[j] * mat2[j * c2 + i];
			}
			send[i] = sum;
		}
		MPI_Send(send, c3, MPI_INT, 0, 1, world);
	}
	if (rank == root) {
		if (num_prs > 0) {
			int start = r1 - num_prs;
			start = start * c3;
			for (i = 1; i <= num_prs; i++) {
				MPI_Recv(&mat3[start], c3, MPI_INT, i, 1, world, &status);
			}
		}
		for (i = 0; i < r3; i++) {
			for (j = 0; j < c3; j++) {
				printf("%d ",mat3[i * c3 + j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
	return 0;
}
