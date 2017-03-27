#include <stdio.h>
#include <stdlib.h>


void readFromFile();
void readFrmoConsole();
void multiplay(int** mat1, int r1, int c1, int** mat2, int r2, int c2);
int checkDimension(int r, int c);
void display(int** mat, int r, int c);
int main() {
    printf("Welcome to vector Matrix multiplication program!\n");
    printf("To read dimensions and values from file press 1\n");
    printf("To read dimensions and values from console press 2\n");
    unsigned short input = 1;
    while(input == 1 || input == 2) {
        scanf("%hu", &input);
        switch(input) {
        case 1: {
            readFromFile();
            break;
        }
        case 2: {
            readFrmoConsole();
            break;
        }
        default:
            printf("Error: Enter a valid input number!!\n");
        }
    }
    return 0;
}

void readFromFile() {
    int r1, r2, c1, c2;
    int* linear1;
    int** mat1;
    int* linear2;
    int** mat2;
    FILE* file = fopen("data.txt", "r");
    fscanf(file, "%d", &r1);
    fscanf(file, "%d", &c1);
    linear1 = malloc(r1 * c1 * sizeof(int));
    mat1 = malloc(sizeof(int*) * r1);
    int i;
    int j;
    for (i = 0 ; i < r1 ; i++) {
        mat1[i] = &linear1[i * c1];
    }
    for (i = 0 ; i < r1 ; i++) {
        for (j = 0 ; j < c1 ; j++) {
            fscanf(file, "%d", &mat1[i][j]);
        }
    }
    fscanf(file, "%d", &r2);
    fscanf(file, "%d", &c2);
    linear2 = malloc(r2 * c2 * sizeof(int));
    mat2 = malloc(sizeof(int*) * r2);
    for (i = 0 ; i < r2 ; i++) {
        mat2[i] = &linear2[i * c2];
    }
    for (i = 0 ; i < r2 ; i++) {
        for (j = 0 ; j < c2 ; j++) {
            fscanf(file, "%d", &mat2[i][j]);
        }
    }
    printf("matrix 1 ==>\n");
    display(mat1, r1, c1);
    printf("matrix 2 ==>\n");
    display(mat2, r2, c2);
    if (checkDimension(r2, c1) != 0) {
        printf("the tow matrices is not able to multiplied!!\ntry again ....");
        readFromFile();
        return;
    }
    printf("Result Matrix is (%dx%d): \n", r1, c2);
    multiplay(mat1, r1, c1, mat2, r2, c2);
    free(mat1[0]);
    free(mat1);
    free(mat2[0]);
    free(mat2);
    fclose(file);
}

void readFrmoConsole() {
    int r1, r2, c1, c2;
    int i;
    int j;
    int* linear1;
    int** mat1;
    int* linear2;
    int** mat2;
    printf("Please enter dimensions of the first matrix: ");
    scanf("%d", &r1);
    scanf("%d", &c1);
    linear1 = malloc(r1 * c1 * sizeof(int));
    mat1 = malloc(sizeof(int*) * r1);
    for (i = 0 ; i < r1 ; i++) {
        mat1[i] = &linear1[i * c1];
    }
    printf("Please enter its elements:");
    for (i = 0 ; i < r1 ; i++) {
        for (j = 0 ; j < c1 ; j++) {
            scanf("%d", &mat1[i][j]);
        }
    }
    printf("Please enter dimensions of the second matrix: ");
    scanf("%d", &r2);
    scanf("%d", &c2);
    linear2 = malloc(r2 * c2 * sizeof(int));
    mat2 = malloc(sizeof(int*) * r2);
    for (i = 0 ; i < r2 ; i++) {
        mat2[i] = &linear2[i * c2];
    }
    printf("Please enter its elements:");
    for (i = 0 ; i < r2 ; i++) {
        for (j = 0 ; j < c2 ; j++) {
            scanf("%d", &mat2[i][j]);
        }
    }
    if (checkDimension(r2, c1) != 0) {
        printf("the tow matrices is not able to multiplied!!\ntry again ....");
        readFrmoConsole();
        return;
    }
    printf("Result Matrix is (%dx%d): \n", r1, c2);
    multiplay(mat1, r1, c1, mat2, r2, c2);
    free(mat1[0]);
    free(mat1);
    free(mat2[0]);
    free(mat2);
}

void multiplay(int** mat1, int r1, int c1, int** mat2, int r2, int c2) {
    int r3 = r1, c3 = c2;
    int* linear3 = malloc(r3 * c3 * sizeof(int));
    int** result = malloc(r3 * sizeof(int*));
    int i, j, k;
    for (i = 0 ; i < r3 ; i++) {
        result[i] = &linear3[i * c3];
    }
    for (i = 0; i < r3; i++) {
        for (j = 0 ; j < c3; j++) {
            int total = 0;
            for (k = 0; k < r2; k++) {
                total += (mat1[i][k] * mat2[k][j]);
            }
            result[i][j] = total;
        }
    }
    display(result, r3, c3);
    free(result[0]);
    free(result);
}

int checkDimension(int r, int c) {
    return r - c;
}

void display(int** mat, int r, int c) {
    int i, j;
    for(i= 0; i < r ; i++) {
        for (j = 0 ; j < c ; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}
