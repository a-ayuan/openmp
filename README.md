# Geometric Mean Calculation with OpenMP Parallelization

This code calculates the geometric mean of an array of `unsigned char` values using various parallelization techniques provided by OpenMP. It demonstrates and times different methods for parallel computing, allowing comparison of each method's performance.

## Included Libraries

```c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <omp.h>
#include <string.h>
```

## Methods

The code includes four main methods for calculating the geometric mean:

1. **Even Split Method**  
   Divides the data evenly among the available threads, assigning each thread a fixed chunk of the array.

   ```c
   double parallel_map_even_split(unsigned char *s, size_t n);
   ```

2. **Task Queue Method**  
   Uses OpenMP's `dynamic` scheduling, where the array is processed in small tasks that are dynamically allocated to threads.

   ```c
   double parallel_map_task_queue(unsigned char *s, size_t n);
   ```

3. **Task Queue with Larger Tasks**  
   Similar to the task queue method, but with a configurable chunk size to adjust the amount of work per task.

   ```c
   double parallel_map_task_queue_large_tasks(unsigned char *s, size_t n, int chunk_size);
   ```

4. **Parallel Map with Schedules**  
   Allows the use of different OpenMP scheduling policies (`static`, `dynamic`, and `guided`) for parallel execution.

   ```c
   double parallel_map_with_schedules(unsigned char *s, size_t n, char* schedule_type);
   ```

## Timing Function

The code uses a helper function, `nsecs()`, to measure the time in nanoseconds for each method's execution, which enables performance comparison.

```c
long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}
```

## Main Execution

The main function reads binary data from specified files, allocates memory dynamically for the array, and computes the geometric mean using each method. Execution times are measured and printed in nanoseconds.

```c
int main(int argc, char *argv[]);
```

### Execution Flow

1. **File Input**  
   Reads binary data from provided file paths.

2. **Method Execution and Timing**  
   Each method is executed in sequence, and its result and execution time are printed.

### Output Example

Each method outputs the geometric mean and the execution time in nanoseconds:

```plaintext
Even Split Method: [result] (Time: [time] ns)
Task Queue Method: [result] (Time: [time] ns)
Task Queue Large Tasks Method: [result] (Time: [time] ns)
Static Schedule Method: [result] (Time: [time] ns)
Dynamic Schedule Method: [result] (Time: [time] ns)
Guided Schedule Method: [result] (Time: [time] ns)
```

## Compilation and Execution

To compile the code:

```bash
gcc -o parallel_geom_mean your_code.c -fopenmp -lm
```

To run the executable:

```bash
./parallel_geom_mean file1 file2 ...
```
