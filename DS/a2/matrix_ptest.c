/*
 --------------------------------------------------
 Project: CP264-a2q3
 File:    matrix_ptest.c
 About:   public test driver
 Author:  HBF
 Version: 2025-08-20
 Compile: gcc matrix.c matrix_ptest.c
 --------------------------------------------------
 */

#include <stdio.h>
#include "matrix.h"

char *fm = "%.2f"; // format string for float number

void display_vector(const char *name, float *v, int n) {
    printf("%s:\n", name);
    for (int i = 0; i < n; i++) {
        printf(fm, v[i]);
        printf(" ");
    }
    printf("\n\n");
}

void display_matrix(const char *name, float *m, int rows, int cols) {
    printf("%s:\n", name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf(fm, m[i * cols + j]);
            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

void test_norm() {
    printf("------------------\nTest: norm\n\n");
    float v[] = {1, 2};
    int n = sizeof(v) / sizeof(float);
    display_vector("v", v, n);
    printf("norm(v): ");
    printf(fm, norm(v, n));
    printf("\n\n");
}

void test_normalize() {
    printf("------------------\nTest: normalize\n\n");
    float v[] = {3, 4};
    int n = sizeof(v) / sizeof(float);
    float vout[n];
    normalize(v, vout, n);
    display_vector("v", v, n);
    display_vector("normalized v", vout, n);
}

void test_matrix_scalar_multiply() {
    printf("------------------\nTest: matrix_scalar_multiply\n\n");
    float A[] = {1, 2, 3, 4};
    float B[4];
    int rows = 2, cols = 2;
    display_matrix("A", A, rows, cols);
    matrix_scalar_multiply(A, 2.0f, B, rows, cols);
    display_matrix("2*A", B, rows, cols);
}

void test_matrix_transpose() {
    printf("------------------\nTest: matrix_transpose\n\n");
    float A[] = {1, 2, 3, 4, 5, 6};
    float B[6];
    int rows = 2, cols = 3;
    display_matrix("A", A, rows, cols);
    matrix_transpose(A, B, rows, cols);
    display_matrix("A^T", B, cols, rows);
}

void test_matrix_add() {
    printf("------------------\nTest: matrix_add\n\n");
    float A[] = {1, 2, 3, 4};
    float B[] = {5, 6, 7, 8};
    float C[4];
    int rows = 2, cols = 2;
    display_matrix("A", A, rows, cols);
    display_matrix("B", B, rows, cols);
    matrix_add(A, B, C, rows, cols);
    display_matrix("A+B", C, rows, cols);
}

void test_matrix_multiply() {
    printf("------------------\nTest: matrix_multiply\n\n");
    float A[] = {1, 2, 3, 4, 5, 6}; // 2x3
    float B[] = {7, 8, 9, 10, 11, 12}; // 3x2
    float C[4]; // 2x2
    int rowsA = 2, colsA = 3, colsB = 2;
    display_matrix("A", A, rowsA, colsA);
    display_matrix("B", B, colsA, colsB);
    matrix_multiply(A, B, C, rowsA, colsA, colsB);
    display_matrix("A*B", C, rowsA, colsB);
}

int main() {
    test_norm();
    test_normalize();
    test_matrix_scalar_multiply();
    test_matrix_transpose();
    test_matrix_add();
    test_matrix_multiply();
    return 0;
}

