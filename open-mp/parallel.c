#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix_functions.h"

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error. Entregar el 'n' y el numero de hilos.\nEjemplo: par 400 4\n");
        return -1;
    }

    srand(time(NULL));
    // Convertir los datos entregados por consola
    int n = atoi(argv[1]);
    int num_of_threads = atoi(argv[2]);
    if (n % num_of_threads != 0) {
        fprintf(stderr, "La división de 'n' entre el numero de hilos debe ser exacta");
        return -1;
    }

    // Inicializar las matrices
    int **A = init_matrix(n, 1);
    int **B = init_matrix(n, 1);
    int **C = init_matrix(n, 0);

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();

    // Calcular multiplicación (con optimización de cache line)
    #pragma omp parallel num_threads(num_of_threads) shared(A, B, C, n)
    {
        #pragma omp for
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int sum = 0;
                for (int k = 0; k < n; k++) {
                    sum += A[i][k] * B[j][k];
                }
                C[i][j] = sum;
            }
        }
    }

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
    FILE *file = fopen("par.csv", "a");
    if (file == NULL) {
        printf("No se puede abrir elapsed_par.csv");
        return -1;
    }
    fprintf(file, "%d, %d, %f\n", num_of_threads, n, elapsed);
    fclose(file);

    return 0;
}
