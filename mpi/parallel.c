#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix_functions.h"

void row_wise_multiplication(int n, int start_row, int end_row, int **A, int **B, int *C) {
    int i, j, k, index, sum;
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < n; j++) {
            index = n * i + j;
            sum = 0;
            for (k = 0; k < n; k++) {
                sum += A[i][k] * B[j][k];
            }
            C[index] = sum;
        }
    }
}

void multiplication(int n, int start_row, int end_row, int **A, int **B, int *C) {
    int i, j, k, index;
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < n; j++) {
            index = n * i + j;
            C[index] = 0;
            for (k = 0; k < n; k++) {
                C[index] += A[i][k] * B[k][j];
            }
        }
    }
}

void print_C(int n, int *C) {
    int i, j, index;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            index = n * i + j;
            printf("%d  ", C[index]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error. Ejecutar dando el 'n' de las matrices y el tipo de  mult.\nEjemplo: seq 400 1");
        return -1;
    }
    srand(time(NULL));
    int n = atoi(argv[1]);
    int mult_type = atoi(argv[2]);

    // Inicializar las matrices
    int **A = init_matrix(n, 1);
    int **B = init_matrix(n, 1);

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes and rank
    int num_processes, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Obtener las filas que calculará cada proceso
    int rows_per_process = n / num_processes;
    int start_row = rows_per_process * rank;
    int end_row = rows_per_process * (rank + 1);

    // Array 1D para que cada proceso guarde su resultado
    int *TEMP = malloc(n * n * sizeof(int));

    // Array 1D para guardar el resultado final
    int *C = NULL;
    if (rank == 0) C = malloc(n * n * sizeof(int));

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();
    MPI_Barrier(MPI_COMM_WORLD);
    double wall_start = MPI_Wtime();

    // Calcular multiplicación
    if (mult_type)
        row_wise_multiplication(n, start_row, end_row, A, B, TEMP);
    else
        multiplication(n, start_row, end_row, A, B, TEMP);

    // Desplazamiento desde donde comienza el resultado en la matriz TEMP
    int offset = start_row * n;
    // Cantidad de valores que cada proceso entrega
    int count = rows_per_process * n;
    MPI_Gather(TEMP + offset, count, MPI_INT, C, count, MPI_INT, 0, MPI_COMM_WORLD);

    // Detener la medición del tiempo y calcular el tiempo transcurrido
    double wall_end = MPI_Wtime();
    double wall_time = wall_end - wall_start;
    double end = get_cpu_time();
    double elapsed = (end - begin);

    if (rank == 0) {
        // Imprimir las matrices
        if (n <= 10) {
            printf("Matriz A:\n");
            print_matrix(n, A);
            printf("Matriz B:\n");
            print_matrix(n, B);
            printf("Matriz C:\n");
            print_C(n, C);
        }

        printf("WTime measured: %.3f seconds.\n", wall_time);
        printf("Time measured: %.3f seconds.\n", elapsed);

        // Escribir resultados en un archivo
        FILE *file;
        if (mult_type)
            file = fopen("parallel.csv", "a");
        else
            file = fopen("par_normal_mult.csv", "a");

        if (file == NULL) {
            printf("No se puede abrir parallel.csv");
            return -1;
        }
        fprintf(file, "%d, %d, %f, %f\n", num_processes, n, elapsed, wall_time);
        fclose(file);
    }

    // Liberar la memoria usada
    free_memory(n, A);
    free_memory(n, B);
    free(TEMP);
    free(C);

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
