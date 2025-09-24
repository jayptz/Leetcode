
#include <math.h>
#include "matrix.h"

/**
 * Calculates the Euclidean norm (length) of a vector.
 * @param v Pointer to the vector.
 * @param n Number of elements in the vector.
 * @return The Euclidean norm of the vector.
 */
float norm(float *v, int n){
    if (n <= 0){
        return 0.0f;
    }
    double sumSquares = 0.0;
    for (int i = 0; i < n; i++){
        sumSquares += (double)v[i] * (double)v[i];
    }
    return (float)sqrt(sumSquares);
}

/**
 * Normalizes a vector (scales it to unit length).
 * @param vin Pointer to the input vector.
 * @param vout Pointer to the output vector (normalized).
 * @param n Number of elements in the vector.
 */
void normalize(const float *vin, float *vout, int n){
    if (n <= 0){
        return;
    }
    double sumSquares = 0.0;
    for (int i = 0; i < n; i++){
        sumSquares += (double)vin[i] * (double)vin[i];
    }
    float mag = (float)sqrt(sumSquares);
    if (mag == 0.0f){
        for (int i = 0; i < n; i++){
            vout[i] = 0.0f;
        }
        return;
    }
    for (int i = 0; i < n; i++){
        vout[i] = vin[i] / mag;
    }
}


/**
 * Multiplies a matrix by a scalar value.
 * @param A Pointer to the input matrix.
 * @param scalar Scalar value to multiply.
 * @param B Pointer to the output matrix (result).
 * @param rows Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 */
void matrix_scalar_multiply(const float *A, float scalar, float *B, int rows, int cols){
    int total = rows * cols;
    for (int i = 0; i < total; i++){
        B[i] = A[i] * scalar;
    }
}

/**
 * Computes the transpose of a matrix.
 * @param A Pointer to the input matrix (rows x cols).
 * @param B Pointer to the output matrix (cols x rows).
 * @param rows Number of rows in the input matrix.
 * @param cols Number of columns in the input matrix.
 */
void matrix_transpose(const float *A, float *B, int rows, int cols){
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            B[c * rows + r] = A[r * cols + c];
        }
    }
}

/**
 * Adds two matrices element-wise.
 * @param A Pointer to the first input matrix.
 * @param B Pointer to the second input matrix.
 * @param C Pointer to the output matrix (sum).
 * @param rows Number of rows in the matrices.
 * @param cols Number of columns in the matrices.
 */
void matrix_add(const float *A, const float *B, float *C, int rows, int cols){
    int total = rows * cols;
    for (int i = 0; i < total; i++){
        C[i] = A[i] + B[i];
    }
}


/* Multiplies two matrices.
 * @param A Pointer to the first input matrix (rowsA x colsA).
 * @param B Pointer to the second input matrix (colsA x colsB).
 * @param C Pointer to the output matrix (rowsA x colsB).
 * @param rowsA Number of rows in matrix A.
 * @param colsA Number of columns in matrix A and rows in matrix B.
 * @param colsB Number of columns in matrix B.
 */
void matrix_multiply(const float *A, const float *B, float *C, int rowsA, int colsA, int colsB){
    for (int r = 0; r < rowsA; r++){
        for (int c = 0; c < colsB; c++){
            double sum = 0.0;
            for (int k = 0; k < colsA; k++){
                sum += (double)A[r * colsA + k] * (double)B[k * colsB + c];
            }
            C[r * colsB + c] = (float)sum;
        }
    }
}