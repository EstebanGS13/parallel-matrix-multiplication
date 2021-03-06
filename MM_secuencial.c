#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "matrix_functions.h"

void multiplication(int n, int **A, int **B, int **C) {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            C[i][j] = 0;
            for (k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error. Ejecutar dando el 'n' de las matrices.\nEjemplo: MM_secuencial 400");
        return -1;
    }

    srand(time(NULL));
    int n = atoi(argv[1]);

    // Inicializar las matrices
    int **A = init_matrix(n, 1);
    int **B = init_matrix(n, 1);
    int **C = init_matrix(n, 0);

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();

    // Calcular multiplicación
    multiplication(n, A, B, C);

    // Detener la medición del tiempo y calcular el tiempo transcurrido
    double end = get_cpu_time();
    double elapsed = (end - begin);

    // Imprimir las matrices
    if (n <= 10) {
        printf("Matriz A:\n");
        print_matrix(n, A);
        printf("Matriz B:\n");
        print_matrix(n, B);
        printf("Matriz C:\n");
        print_matrix(n, C);
    }

    printf("Time measured: %.3f seconds.\n", elapsed);

    // Liberar la memoria usada
    free_memory(n, A);
    free_memory(n, B);
    free_memory(n, C);

    // Escribir resultados en un archivo
    FILE *file = fopen("elapsed_seq.csv", "a");
    if (file == NULL) {
        printf("No se puede abrir elapsed_seq.csv");
        return -1;
    }
    fprintf(file, "%d, %f\n", n, elapsed);
    fclose(file);

    return 0;
}
