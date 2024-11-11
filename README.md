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
