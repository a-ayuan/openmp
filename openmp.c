#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <omp.h>
#include <string.h>

double parallel_map_even_split(unsigned char *s, size_t n);
double parallel_map_task_queue(unsigned char *s, size_t n);
double parallel_map_task_queue_large_tasks(unsigned char *s, size_t n, int chunk_size);
double parallel_map_with_schedules(unsigned char *s, size_t n, char* schedule_type);

long long nsecs() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

// Computes the geometric mean using the Even Split method
double parallel_map_even_split(unsigned char *s, size_t n) {
    double sum = 0;
    int num_threads = omp_get_max_threads();
    int chunk_size = n / num_threads;

    #pragma omp parallel
    {
        int start = omp_get_thread_num() * chunk_size;
        int end = (omp_get_thread_num() == num_threads - 1) ? n : start + chunk_size;
        
        for (int i = start; i < end; i++) {
            if (s[i] > 0) {
                #pragma omp atomic
                sum += log(s[i]);
            }
        }
    }
    return exp(sum / n);
}

// Computes the geometric mean using the Task Queue method
double parallel_map_task_queue(unsigned char *s, size_t n) {
    double sum = 0;

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < n; i++) {
        if (s[i] > 0) {
            #pragma omp atomic
            sum += log(s[i]) / n;
        }
    }
    return exp(sum);
}

// Computes the geometric mean using Task Queue with Larger Tasks
double parallel_map_task_queue_large_tasks(unsigned char *s, size_t n, int chunk_size) {
    double sum = 0;

    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (int i = 0; i < n; i++) {
        if (s[i] > 0) {
            #pragma omp atomic
            sum += log(s[i]) / n;
        }
    }
    return exp(sum);
}

// Computes the geometric mean using OpenMP's parallel for schedules
double parallel_map_with_schedules(unsigned char *s, size_t n, char* schedule_type) {
    double sum = 0;

    if (strcmp(schedule_type, "static") == 0) {
        #pragma omp parallel for schedule(static)
        for (int i = 0; i < n; i++) {
            if (s[i] > 0) {
                #pragma omp atomic
                sum += log(s[i]) / n;
            }
        }
    } else if (strcmp(schedule_type, "dynamic") == 0) {
        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < n; i++) {
            if (s[i] > 0) {
                #pragma omp atomic
                sum += log(s[i]) / n;
            }
        }
    } else if (strcmp(schedule_type, "guided") == 0) {
        #pragma omp parallel for schedule(guided)
        for (int i = 0; i < n; i++) {
            if (s[i] > 0) {
                #pragma omp atomic
                sum += log(s[i]) / n;
            }
        }
    }
    return exp(sum);
}

// Main function
int main(int argc, char *argv[]) {
    char *s = NULL;
    size_t n = 0;

    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            size_t size = ftell(f);
            fseek(f, 0, SEEK_SET);
            s = realloc(s, n + size);
            fread(s + n, 1, size, f);
            fclose(f);
            n += size;
        } else {
            errno = 0;
        }
    }

    // Compute and time each method
    long long t0, t1;
    
    t0 = nsecs();
    double result_even_split = parallel_map_even_split((unsigned char*) s, n);
    t1 = nsecs();
    printf("Even Split Method: %g (Time: %lld ns)\n", result_even_split, t1 - t0);

    t0 = nsecs();
    double result_task_queue = parallel_map_task_queue((unsigned char*) s, n);
    t1 = nsecs();
    printf("Task Queue Method: %g (Time: %lld ns)\n", result_task_queue, t1 - t0);

    t0 = nsecs();
    double result_task_queue_large = parallel_map_task_queue_large_tasks((unsigned char*) s, n, 10);
    t1 = nsecs();
    printf("Task Queue Large Tasks Method: %g (Time: %lld ns)\n", result_task_queue_large, t1 - t0);

    t0 = nsecs();
    double result_static_schedule = parallel_map_with_schedules((unsigned char*) s, n, "static");
    t1 = nsecs();
    printf("Static Schedule Method: %g (Time: %lld ns)\n", result_static_schedule, t1 - t0);

    t0 = nsecs();
    double result_dynamic_schedule = parallel_map_with_schedules((unsigned char*) s, n, "dynamic");
    t1 = nsecs();
    printf("Dynamic Schedule Method: %g (Time: %lld ns)\n", result_dynamic_schedule, t1 - t0);

    t0 = nsecs();
    double result_guided_schedule = parallel_map_with_schedules((unsigned char*) s, n, "guided");
    t1 = nsecs();
    printf("Guided Schedule Method: %g (Time: %lld ns)\n", result_guided_schedule, t1 - t0);

    free(s);
    return 0;
}
