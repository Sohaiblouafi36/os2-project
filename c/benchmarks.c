#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "asm_functions.h"

#define ITERATIONS 10000000
#define WARMUP_RUNS 100
#define ARRAY_SIZE 1000
#define CACHE_LINE_SIZE 64

// C implementations for comparison
long factorial_c(int num) {
    long result = 1;
    for (int i = 2; i <= num; i++) {
        result *= i;
    }
    return result;
}

size_t strlen_c(const char *str) {
    size_t len = 0;
    while (*str++) len++;
    return len;
}

void strrev_c(char *str) {
    char *end = str + strlen_c(str) - 1;
    while (str < end) {
        char tmp = *str;
        *str++ = *end;
        *end-- = tmp;
    }
}

void reverse_array_c(int *arr, size_t len) {
    for (size_t i = 0; i < len/2; i++) {
        int tmp = arr[i];
        arr[i] = arr[len-1-i];
        arr[len-1-i] = tmp;
    }
}

int find_second_largest_c(int *arr, size_t len) {
    if (len < 2) return -1;
    
    int first = arr[0], second = arr[1];
    if (first < second) {
        int tmp = first;
        first = second;
        second = tmp;
    }
    
    for (size_t i = 2; i < len; i++) {
        if (arr[i] > first) {
            second = first;
            first = arr[i];
        } else if (arr[i] > second) {
            second = arr[i];
        }
    }
    return second;
}

// Improved benchmarking infrastructure
void run_benchmark(const char *name, void (*asm_func)(void), void (*c_func)(void)) {
    struct timespec start, end;
    long asm_ns, c_ns;
    
    // Warm-up runs to avoid cold cache effects
    for (int i = 0; i < WARMUP_RUNS; i++) {
        asm_func();
        c_func();
    }
    
    // Benchmark ASM
    clock_gettime(CLOCK_MONOTONIC, &start);
    asm_func();
    clock_gettime(CLOCK_MONOTONIC, &end);
    asm_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    
    // Benchmark C
    clock_gettime(CLOCK_MONOTONIC, &start);
    c_func();
    clock_gettime(CLOCK_MONOTONIC, &end);
    c_ns = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    
    // Results
    printf("%-20s ASM: %8ld ns | C: %8ld ns | Ratio: %5.2fx | %s\n",
           name, asm_ns, c_ns,
           (double)c_ns / (double)asm_ns,
           asm_ns < c_ns ? "FASTER" : "slower");
}

// Benchmark test cases
void benchmark_factorial() {
    int num = 10;
    volatile long result_asm = 0, result_c = 0;
    
    void asm_test(void) {
        for (int i = 0; i < ITERATIONS; i++) {
            result_asm = factorial_asm(num);
        }
    }
    
    void c_test(void) {
        for (int i = 0; i < ITERATIONS; i++) {
            result_c = factorial_c(num);
        }
    }
    
    run_benchmark("factorial", asm_test, c_test);
}

void benchmark_strlen() {
    const char *str = "This is a test string for benchmarking";
    volatile size_t result_asm = 0, result_c = 0;
    
    void asm_test(void) {
        for (int i = 0; i < ITERATIONS; i++) {
            result_asm = strlen_asm(str);
        }
    }
    
    void c_test(void) {
        for (int i = 0; i < ITERATIONS; i++) {
            result_c = strlen(str);
        }
    }
    
    run_benchmark("strlen", asm_test, c_test);
}

void benchmark_strrev() {
    char str_asm[50] = "A string to reverse";
    char str_c[50] = "A string to reverse";
    volatile int dummy = 0;
    
    void asm_test(void) {
        for (int i = 0; i < ITERATIONS/10; i++) {
            strcpy(str_asm, "A string to reverse");
            strrev_asm(str_asm);
            dummy += str_asm[0]; // Prevent optimization
        }
    }
    
    void c_test(void) {
        for (int i = 0; i < ITERATIONS/10; i++) {
            strcpy(str_c, "A string to reverse");
            strrev_c(str_c);
            dummy += str_c[0]; // Prevent optimization
        }
    }
    
    run_benchmark("strrev", asm_test, c_test);
}

void benchmark_reverse_array() {
    int *arr_asm = aligned_alloc(CACHE_LINE_SIZE, ARRAY_SIZE * sizeof(int));
    int *arr_c = aligned_alloc(CACHE_LINE_SIZE, ARRAY_SIZE * sizeof(int));
    volatile int dummy = 0;
    
    // Initialize array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr_asm[i] = arr_c[i] = i;
    }
    
    void asm_test(void) {
        for (int i = 0; i < ITERATIONS/100; i++) {
            reverse_array_asm(arr_asm, ARRAY_SIZE);
            dummy += arr_asm[0]; // Prevent optimization
        }
    }
    
    void c_test(void) {
        for (int i = 0; i < ITERATIONS/100; i++) {
            reverse_array_c(arr_c, ARRAY_SIZE);
            dummy += arr_c[0]; // Prevent optimization
        }
    }
    
    run_benchmark("reverse_array", asm_test, c_test);
    free(arr_asm);
    free(arr_c);
}

void benchmark_second_largest() {
    int *arr_asm = aligned_alloc(CACHE_LINE_SIZE, ARRAY_SIZE * sizeof(int));
    int *arr_c = aligned_alloc(CACHE_LINE_SIZE, ARRAY_SIZE * sizeof(int));
    volatile int result_asm = 0, result_c = 0;
    
    // Initialize array with random values
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr_asm[i] = arr_c[i] = rand() % 1000;
    }
    
    void asm_test(void) {
        for (int i = 0; i < ITERATIONS/10; i++) {
            result_asm = find_second_largest_asm(arr_asm, ARRAY_SIZE);
        }
    }
    
    void c_test(void) {
        for (int i = 0; i < ITERATIONS/10; i++) {
            result_c = find_second_largest_c(arr_c, ARRAY_SIZE);
        }
    }
    
    run_benchmark("second_largest", asm_test, c_test);
    free(arr_asm);
    free(arr_c);
}

int main() {
    printf("\nBenchmark Results (Iterations: %d, Array Size: %d)\n", ITERATIONS, ARRAY_SIZE);
    printf("------------------------------------------------\n");
    
    benchmark_factorial();
    benchmark_strlen();
    benchmark_strrev();
    benchmark_reverse_array();
    benchmark_second_largest();
    
    printf("\nNotes:\n");
    printf("- Times are total nanoseconds for all iterations\n");
    printf("- Ratio >1 means ASM is faster, <1 means slower\n");
    printf("- Warmup runs: %d, Cache line size: %d bytes\n", WARMUP_RUNS, CACHE_LINE_SIZE);
    printf("- Array operations use fewer iterations due to higher overhead\n");
    
    return 0;
}
