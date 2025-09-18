/* 
--------------------------------------------------
Project: a1q2
File:    powersum_ptest.c
Author:  HBF
Version: 2025-01-09
--------------------------------------------------
*/

#include <stdio.h>
#include "powersum.h"

int bases[] = {2, 3};
int exp_tests[]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 31, 32};
int tests[] = {1, 2, 3, 4, 5, 6};

void test_power_overflow(void) {
    printf("------------------\n");
    printf("Test: power_overflow(base,exponent)\n\n");
    int base_count = sizeof bases / sizeof *bases;
    int exp_count = sizeof exp_tests / sizeof *exp_tests;
    for (int j=0; j<base_count; j++) {
        for(int i = 8; i < exp_count; i++) {
            if (i % 2 == 1) {
            printf("%s(%d %d): %d", "power_overflow", bases[j], exp_tests[i], power_overflow(bases[j], exp_tests[i]));
            printf("\n");
            }
        }   
    }
    printf("\n");
}


void test_mypower(void) {
    printf("------------------\n");
    printf("Test: mypower(base,exponent)\n\n");
    int base_count = sizeof bases / sizeof *bases;
    int exp_count = sizeof exp_tests / sizeof *exp_tests;
    for (int j=0; j<base_count; j++) {
    for(int i = 0; i < exp_count; i++) {
        if (i % 2 == 1) {
            printf("%s(%d,%d): %d", "mypower", bases[j], exp_tests[i], mypower(bases[j], exp_tests[i]));
            printf("\n");
        }
    }
    }
    printf("\n");
}

void test_powersum(void) {
    printf("------------------\n");
    printf("Test: powersum(base,n)\n\n");
    int base_count = sizeof bases / sizeof *bases;
    int exp_count = sizeof exp_tests / sizeof *exp_tests;
    for (int j=0; j<base_count; j++) {
    for(int i = 0; i < exp_count; i++) {
        printf("%s(%d,%d): %d", "powersum", bases[j], exp_tests[i], powersum(bases[j], exp_tests[i]));
        printf("\n");
    }
    }
    printf("\n");
}

int main(int argc, char *args[])
{
	test_power_overflow();
    test_mypower();
    test_powersum();
	return 0;
}
