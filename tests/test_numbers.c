#include <stdio.h>
#include <assert.h>
#include "asm_functions.h"

void test_factorial() {
    printf("Testing factorial_asm:\n");
    
    struct {
        int input;
        long expected;
    } test_cases[] = {
        {0, 1},          // 0! = 1
        {1, 1},          // 1! = 1
        {5, 120},        // 5! = 120
        {10, 3628800},   // 10! = 3,628,800
        {12, 479001600}  // 12! = 479,001,600 (max 32-bit)
    };
    
    for (size_t i = 0; i < sizeof(test_cases)/sizeof(test_cases[0]); i++) {
        long result = factorial_asm(test_cases[i].input);
        printf("  %2d! => %10ld (expected %10ld) - ",
               test_cases[i].input, result, test_cases[i].expected);
        assert(result == test_cases[i].expected);
        printf("✓\n");
    }
}

int main() {
    test_factorial();
    printf("\nAll factorial tests passed!\n");
    return 0;
}
