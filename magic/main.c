#include <stdio.h>
#include <stdbool.h>

extern int isMagic(int num); // NASM version

bool isMagic_c(int num) {
    while (num > 9) {
        int sum = 0;
        while (num != 0) {
            sum += num % 10;
            num /= 10;
        }
        num = sum;
    }
    return num == 1;
}

int main() {
    int testNumber = 19;

    int result_c = isMagic_c(testNumber);
    int result_asm = isMagic(testNumber);

    printf("Testing number: %d\n", testNumber);
    printf("C version:    %s\n", result_c ? "Magic" : "Not Magic");
    printf("NASM version: %s\n", result_asm ? "Magic" : "Not Magic");

    return 0;
}

