/*
 * your program signature
 */ 
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
 
float horner(float *p, int n, float x);

void derivative(float *p, float *d, int n);

float newton(float *p, int n, float x0);

#endif