#include <processthreadsapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int **init_matrix(int n, int fill) {
    // Crea un puntero a punteros (por cada fila)
    int **M = malloc(n * sizeof(int *));

    int i, j;
    for (i = 0; i < n; i++) {
        // Asigna memoria para las columnas de cada fila
        M[i] = malloc(n * sizeof(int *));

        // Si hay que llenar la matriz
        if (fill) {
            for (j = 0; j < n; j++) {
                M[i][j] = 1 + rand() % 10;
            }
        }
    }
    return M;
}

void print_matrix(int n, int **M) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d  ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

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

double get_cpu_time() {
    FILETIME a, b, c, d;
    if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
        //  Returns total user time.
        //  Can be tweaked to include kernel times as well.
        return (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    } else {
        //  Handle error
        printf("Error while calculating CPU time.");
        return 0;
    }
}

void free_memory(int n, int **M) {
    int i;
    for (i = 0; i < n; i++) {
        free(M[i]);
    }
    free(M);
    M = NULL;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Ejecutar dando el 'n' de las matrices, ejemplo: MM_secuencial 400");
        return -1;
    }

    srand(time(NULL));
    int n = atoi(argv[1]);

    // Inicializar las matrices
    int **A = init_matrix(n, 1);
    int **B = init_matrix(n, 1);

    int **C = init_matrix(n, 0);

    // Imprimir las matrices
    // print_matrix(n, A);
    // print_matrix(n, B);

    // Comenzar a medir el tiempo
    double begin = get_cpu_time();

    // Calcular multiplicación
    multiplication(n, A, B, C);
    // print_matrix(n, C);

    // Detener la medición del tiempo y calcular el tiempo transcurrido
    double end = get_cpu_time();
    double elapsed = (end - begin);

    printf("Time measured: %.3f seconds.\n", elapsed);

    // Liberar la memoria usada
    free_memory(n, A);
    free_memory(n, B);
    free_memory(n, C);

    // Escribir resultados en un archivo
    FILE *file = fopen("elapsed.csv", "a");
    if (file == NULL) {
        printf("No se puede abrir elapsed.csv");
        return -1;
    }
    fprintf(file, "%d, %f\n", n, elapsed);
    fclose(file);

    return 0;
}
