#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix_functions.h"

int n, num_threads;
int **A, **B, **C;

void *multiplication(void *arg) {
    int th_id, parts, start_row, end_row, i, j, k;
    th_id = *((int *)arg);

    parts = n / num_threads;
    start_row = parts * th_id;
    end_row = parts * (th_id + 1);

    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error. Entregar el 'n' y el numero de hilos.\nEjemplo: MM_paralela 400 4");
        return -1;
    }

    srand(time(NULL));
    n = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    if (n % num_threads != 0) {
        fprintf(stderr, "La división de 'n' entre el numero de hilos debe ser exacta");
        return -1;
    }

    // Inicializar las matrices
    A = init_matrix(n, 1);
    B = init_matrix(n, 1);
    C = init_matrix(n, 0);

    // Crear los hilos
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    pthread_attr_t attribute;
    pthread_attr_init(&attribute);
    pthread_attr_setdetachstate(&attribute, PTHREAD_CREATE_JOINABLE);

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();

    // Calcular multiplicación
    for (int i = 0; i < num_threads; i++) {
        int *th_id = malloc(sizeof(int));
        *th_id = i;
        pthread_create(&threads[i], &attribute, multiplication, (void *)th_id);
    }

    // Unir los hilos
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
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

    pthread_attr_destroy(&attribute);

    // Liberar la memoria usada
    free_memory(n, A);
    free_memory(n, B);
    free_memory(n, C);

    // Escribir resultados en un archivo
    FILE *file = fopen("elapsed_par.csv", "a");
    if (file == NULL) {
        printf("No se puede abrir elapsed_par.csv");
        return -1;
    }
    fprintf(file, "%d, %d, %f\n", num_threads, n, elapsed);
    fclose(file);

    pthread_exit(NULL);
    return 0;
}
