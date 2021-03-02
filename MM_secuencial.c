#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <processthreadsapi.h>

void fill(int n, int A[n][n], int B[n][n])
{
	srand(time(NULL));

	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			A[i][j] = 1 + rand() % (11 - 1);
			B[i][j] = 1 + rand() % (11 - 1);
		}
	}
}

void print(int n, int M[n][n])
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%d  ", M[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

void multiplication(int n, int A[n][n], int B[n][n], int C[n][n])
{
	int i, j, k;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			C[i][j] = 0;
			for (k = 0; k < n; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

double get_cpu_time()
{
	FILETIME a, b, c, d;
	if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0)
	{
		//  Returns total user time.
		//  Can be tweaked to include kernel times as well.
		return (double)(d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
	}
	else
	{
		//  Handle error
		printf("Error while calculating CPU time.");
		return 0;
	}
}

int main(int argc, char const *argv[])
{
	int n, i, j;
	printf("Ingrese el orden de la matriz: ");
	scanf("%d", &n);

	int A[n][n];
	int B[n][n];
	int C[n][n];

	// Llenar las matrices
	fill(n, A, B);

	// Imprimir las matrices
	// print(n, A);
	// print(n, B);

    // Start measuring time
    double begin = get_cpu_time();

	// Calcular multiplicación
	multiplication(n, A, B, C);
	// print(n, C);

	// Stop measuring time and calculate the elapsed time
    double end = get_cpu_time();
    double elapsed = (end - begin);
	printf("result: %f", elapsed);
    printf("Time measured: %.3f seconds.\n", elapsed);

	return 0;
}
