#include <stdio.h>
#include <stdlib.h>

// #ifdef _WIN32
// #include <processthreadsapi.h>

// double get_cpu_time() {
//     FILETIME a, b, c, d;
//     if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
//         //  Returns total user time.
//         //  Can be tweaked to include kernel times as well.
//         return (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
//     } else {
//         //  Handle error
//         printf("Error while calculating CPU time.");
//         return 0;
//     }
// }

// #else
#include <time.h>

double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

// #endif

int **init_matrix(int n, int fill) {
    // Crea un puntero a punteros (por cada fila)
    int **M = (int **)malloc(n * sizeof(int *));

    int i, j;
    for (i = 0; i < n; i++) {
        // Asigna memoria para las columnas de cada fila
        M[i] = (int *)malloc(n * sizeof(int *));

        // Si hay que llenar la matriz
        if (fill) {
            for (j = 0; j < n; j++) {
                M[i][j] = 1 + rand() % 10;
            }
        }
        else {
            for (j = 0; j < n; j++) {
                M[i][j] = 0;
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

void free_memory(int n, int **M) {
    int i;
    for (i = 0; i < n; i++) {
        free(M[i]);
    }
    free(M);
    M = NULL;
}
