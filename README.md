# Parallel Matrix Multiplication

This project contains different ways to improve the performance of the matrix multiplication algorithm. We take two matrices of size n\*n with random filled values and calculate their multiplication, we then compare the CPU time taken to complete only this task in both, the sequential and the parallel implementation.

Each subdirectory inside the main directory is like a project on its own, in each one, we try different approaches like threads, processes, optimizations, etc. They also have a similar structure:

- `bin/` where the binary files are stored.
- `results/` includes the `.csv` files that contain the CPU time and other data.
- `sequential.c` the sequential implementation of the algorithm.
- `parallel.c` the parallel implementation.
- `matrix_functions.h` common functions used by the `.c` files.
- `run.sh` a bash script to run the tests.

## Installation

A UNIX-based system is required since many libraries don't work on Windows, however, you can install [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) on Win10.

Install the gcc compiler:

```bash
sudo apt-get install gcc
```

## Usage

Inside any `bin/` directory, to execute the files run the commands:

- For `seq`, type `./seq <n>`, replace `n` with the number of rows.

  ```sh
  ./seq 1000
  ```

  Some files will requiere an additional argument for the multiplication type: `0` for normal or `1` for row wise multiplication.

  ```sh
  ./seq 1000 1
  ```

- For `par`, type `./par <n> <threads>`, replace `threads` with the number of threads to use.

  ```sh
  ./par 1000 4
  ```

If you want to compile the code again on your machine, check the `run.sh` files to see the commands used.
