#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "matrix_functions.h"

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
        fprintf(stderr, "Error. Entregar el 'n' y el numero de procesos.\nEjemplo: par 400 4");
        return -1;
    }

    srand(time(NULL));

    // Convertir los datos entregados por consola
    int n = atoi(argv[1]);
    int num_processes = atoi(argv[2]);
    if (n % num_processes != 0) {
        fprintf(stderr, "La división de 'n' entre el numero de procesos debe ser exacta");
        return -1;
    }
    int parts = n / num_processes;  // Partes en que serán divididas las filas de la matriz C

    // Inicializar las matrices
    int **A = init_matrix(n, 1);
    int **B = init_matrix(n, 1);

    // 1. Crear segmento de memoria compartida
    int segment = shmget(IPC_PRIVATE, (n * n * sizeof(int)), IPC_CREAT | 0666);
    if (segment < 0) {
        perror("shmget error");
        exit(-1);
    }
    printf("Allocated %d, at id %d\n", (int)(n * n * sizeof(int)), segment);

    // 2. Crear los procesos hijos
    int parent_id = getpid();
    int process;
    for (int i = 0; i < num_processes; i++) {
        if (fork() == 0) {  // Si se ejecuta el proceso hijo
            process = i;
            break;
        }
    }

    // 3. Adjuntar el segmento a los procesos
    int *shared_C = shmat(segment, NULL, 0);
    if (shared_C == (void *)-1) {
        perror("shmat error");
        exit(-1);
    }
    printf("Process %d attached the segment %d\n", getpid(), segment);

    if (getpid() != parent_id) {  // Se ejecuta un proceso hijo
        // 4. Hacer la multiplicación
        int start_row = parts * process;
        int end_row = parts * (process + 1);
        multiplication(n, start_row, end_row, A, B, shared_C);

        // 5. Separar el segmento del proceso hijo
        int detach = shmdt(shared_C);
        if (detach < 0) {
            perror("shmdt error");
            exit(-1);
        }
        printf("Process %d detached the segment %d\n", getpid(), segment);

        // 6. Terminar el proceso
        return 0;

    } else {
        // Se ejecuta el proceso padre
        for (int i = 0; i < num_processes; i++) {
            wait(NULL);  // Esperar a que los procesos terminen
        }
    }

    // Medir el CPU time
    struct rusage usage;
    struct timeval cpu_time;
    getrusage(RUSAGE_CHILDREN, &usage);
    cpu_time = usage.ru_utime;

    // Imprimir las matrices
    if (n <= 10) {
        printf("Matriz A:\n");
        print_matrix(n, A);
        printf("Matriz B:\n");
        print_matrix(n, B);
        printf("Matriz C:\n");
        print_C(n, shared_C);
    }
    printf("Time measured: %ld.%ld seconds.\n", cpu_time.tv_sec, cpu_time.tv_usec);

    // Escribir resultados en un archivo
    FILE *file = fopen("parallel.csv", "a");
    if (file == NULL) {
        printf("No se puede abrir parallel.csv");
        return -1;
    }
    fprintf(file, "%d, %d, %ld.%ld\n", num_processes, n, cpu_time.tv_sec, cpu_time.tv_usec);
    fclose(file);

    // Separar el segmento del proceso padre
    int detach = shmdt(shared_C);
    if (detach < 0) {
        perror("shmdt error");
        exit(-1);
    }
    printf("Process %d detached the segment %d\n", getpid(), segment);

    // 7. Borrar el segmento
    int delete = shmctl(segment, IPC_RMID, NULL);
    if (delete < 0) {
        perror("shmctl error");
        exit(-1);
    }

    // Liberar la memoria usada
    free_memory(n, A);
    free_memory(n, B);
    free(C);

    return 0;
}
