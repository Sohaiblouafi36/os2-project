#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asm_functions.h"

// C implementations for verification
long factorial_c(int num) {
    long result = 1;
    for (int i = 2; i <= num; i++) {
        result *= i;
    }
    return result;
}
// C implementations for verification
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

void print_array(int *arr, size_t len) {
    printf("[");
    for (size_t i = 0; i < len; i++) {
        printf("%d", arr[i]);
        if (i < len - 1) printf(", ");
    }
    printf("]");
}

void demo_factorial() {
    printf("\n=== Factorial Demo ===\n");
    
    int numbers[] = {0, 1, 5, 10, 12}; // 12! is largest that fits in 32-bit
    size_t count = sizeof(numbers)/sizeof(numbers[0]);
    
    for (size_t i = 0; i < count; i++) {
        int num = numbers[i];
        long asm_result = factorial_asm(num);
        long c_result = factorial_c(num);
        
        printf("Number: %2d | ASM: %10ld | C: %10ld | %s\n",
               num, asm_result, c_result,
               asm_result == c_result ? "✓" : "✗");
    }
}


void demo_string_operations() {
    printf("\n=== String Operations Demo ===\n");
    
    char strings[][50] = {"", "a", "hello", "racecar", "12345"};
    size_t count = sizeof(strings)/sizeof(strings[0]);
    
    for (size_t i = 0; i < count; i++) {
        char asm_str[50], c_str[50];
        strcpy(asm_str, strings[i]);
        strcpy(c_str, strings[i]);
        
        // Length
        size_t asm_len = strlen_asm(asm_str);
        size_t c_len = strlen_c(asm_str);
        
        // Reverse
        strrev_asm(asm_str);
        strrev_c(c_str);
        
        printf("Original: '%s'\n", strings[i]);
        printf("  Length - ASM: %zu | C: %zu\n", asm_len, c_len);
        printf("  Reversed - ASM: '%s' | C: '%s'\n", asm_str, c_str);
        printf("  %s\n\n", strcmp(asm_str, c_str) == 0 ? "✓" : "✗");
    }
}

void demo_array_operations() {
    printf("\n=== Array Operations Demo ===\n");
    
    int test_arrays[][5] = {
        {1}, 
        {1, 2}, 
        {1, 2, 3}, 
        {1, 2, 3, 4}, 
        {5, 2, 9, 1, 7}
    };
    size_t lengths[] = {1, 2, 3, 4, 5};
    size_t count = sizeof(lengths)/sizeof(lengths[0]);
    
    for (size_t i = 0; i < count; i++) {
        size_t len = lengths[i];
        int *asm_arr = malloc(len * sizeof(int));
        int *c_arr = malloc(len * sizeof(int));
        
        memcpy(asm_arr, test_arrays[i], len * sizeof(int));
        memcpy(c_arr, test_arrays[i], len * sizeof(int));
        
        reverse_array_asm(asm_arr, len);
        reverse_array_c(c_arr, len);
        
        printf("Original: ");
        print_array(test_arrays[i], len);
        printf("\n  ASM reversed: ");
        print_array(asm_arr, len);
        printf("\n  C reversed:   ");
        print_array(c_arr, len);
        
        int match = memcmp(asm_arr, c_arr, len * sizeof(int)) == 0;
        printf("\n  %s\n\n", match ? "✓" : "✗");
        
        free(asm_arr);
        free(c_arr);
    }
}

int main() {
    printf("===== Assembly Function Demonstrations =====\n");
    
    demo_factorial();
    demo_string_operations();
    demo_array_operations();
    
    printf("\nNote:\n");
    printf("✓ indicates ASM and C implementations agree\n");
    printf("✗ indicates a discrepancy between implementations\n");
    
    return 0;
}
