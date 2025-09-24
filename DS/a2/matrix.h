/*
 * your program signature
 */
 
 #ifndef MATRIX_H
 #define MATRIX_H
 
 float norm(float *v, int n);
 
 void normalize(const float *vin, float *vout, int n);
 
 void matrix_scalar_multiply(const float *A, float scalar, float *B, int rows, int cols);
 
 void matrix_transpose(const float *A, float *B, int rows, int cols);
 
 void matrix_add(const float *A, const float *B, float *C, int rows, int cols);
 
 void matrix_multiply(const float *A, const float *B, float *C, int rowsA, int colsA, int colsB);
 
 #endif