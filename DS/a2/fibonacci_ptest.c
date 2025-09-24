/**
 * -------------------------------------
 * @file  fibonacci_ptest.c
 * @brief test driver
 * -------------------------------------
 * @author Hongbing Fan, 123456789, hfan@wlu.ca
 *
 * @version 2025-08-19
 *
 * -------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "fibonacci.h"

const int tests[] = {0, 1, 2, 3, 4, 46, 47};

void test_iterative_fibonacci()
{
	printf("------------------\n");
	printf("Test: iterative_fibonacci(n)\n\n");
	int count = sizeof tests / sizeof(int);
	int i;
	for (i = 0; i < count; i++)
	{
		int f = iterative_fibonacci(tests[i]);
		if (f != -1)
			printf("iterative_fibonacci(%d): %d\n", tests[i], f);
		else
			printf("iterative_fibonacci(%d): overflow\n", tests[i]);
	}
	printf("\n");
}

void test_recursive_fibonacci()
{
	printf("------------------\n");
	printf("Test: recursive_fibonacci(n)\n\n");
	int count = sizeof tests / sizeof(int);
	int i;
	for (i = 0; i < count-2; i++)
	{
		int f = recursive_fibonacci(tests[i]);
		if (f != -1)
			printf("recursive_fibonacci(%d): %d\n", tests[i], f);
		else
			printf("recursive_fibonacci(%d): overflow\n", tests[i]);
	}
	printf("\n");
}

void test_dpbu_fibonacci()
{
	printf("------------------\n");
	printf("Test: dpbu_fibonacci(n)\n\n");
	int count = sizeof tests / sizeof(int);
	int a[100];
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 100; j++)
			a[j] = 0;
		
		int f = dpbu_fibonacci(a, tests[i]);
		if (f != -1)
			printf("dpbu_fibonacci(%d): %d\n", tests[i], f);
		else
			printf("dpbu_fibonacci(%d): overflow\n", tests[i]);
	}
	printf("\n");
}

void test_dptd_fibonacci()
{
	printf("------------------\n");
	printf("Test: dptd_fibonacci(n)\n\n");
	int count = sizeof tests / sizeof(int);
	int a[100];
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < 100; j++)
			a[j] = 0;

		int f = dptd_fibonacci(a, tests[i]);
		if (f != -1)
			printf("dptd_fibonacci(%d): %d\n", tests[i], f);
		else
			printf("dptd_fibonacci(%d): overflow\n", tests[i]);
	}
	printf("\n");
}

void time_test_fibonacci()
{
	printf("------------------\n");
	printf("Test: time, comparison\n\n");

	int n = 40, f[n + 1];
	printf("iterative_fibonacci(%d): %d\n", n, iterative_fibonacci(n));
	printf("recursive_fibonacci(%d): %d\n", n, recursive_fibonacci(n));
	for (int j = 0; j < n; j++)
		f[j] = -1;
	printf("dpbu_fibonacci(%d): %d\n", n, dpbu_fibonacci(f, n));
	for (int j = 0; j < n; j++)
		f[j] = -1;
	printf("dptd_fibonacci(%d): %d\n", n, dptd_fibonacci(f, n));

	printf("\n**Function runtime measurement**\n");
	clock_t t1, t2;
	int m1 = 500000;
	t1 = clock();
	for (int i = 0; i < m1; i++)
	{
		iterative_fibonacci(n);
	}
	t2 = clock();
	double time_span1 = (double)t2 - t1;
	printf("time_span(iterative_fibonacci(%d) for %d times):%0.1f (ms)\n", n, m1, time_span1);

	int m2 = 5;
	t1 = clock();
	for (int i = 0; i < m2; i++)
	{
		recursive_fibonacci(n);
	}
	t2 = clock();
	double time_span2 = t2 - t1;
	printf("time_span(recursive_fibonacci(%d) for %d times):%0.1f (ms)\n", n, m2, time_span2);

	int m3 = 500000;
	t1 = clock();
	for (int i = 0; i < m3; i++)
	{
		for (int j = 0; j <= n; j++)
			f[j] = 0;
		dpbu_fibonacci(f, n);
	}
	t2 = clock();
	double time_span3 = (double)t2 - t1;
	printf("time_span(dpbu_fibonacci(%d) for %d times):%0.1f (ms)\n", n, m3, time_span3);

	int m4 = 50000;
	t1 = clock();
	for (int i = 0; i < m4; i++)
	{
		for (int j = 0; j <= n; j++)
			f[j] = 0;
		dptd_fibonacci(f, n);
	}
	t2 = clock();
	double time_span4 = (double)t2 - t1;
	printf("time_span(dptd_fibonacci(%d) for %d times):%0.1f (ms)\n", n, m4, time_span4);
	printf("\n\n**Comparisons**\n");
	printf("time_span(recursive_fibonacci(%d))/time_span(iterative_fibonacci(%d)):%0.1f\n", n, n, (time_span2 / time_span1) * (m1 / m2));
	printf("time_span(dpbu_fibonacci(%d))/time_span(iterative_fibonacci(%d)):%0.1f\n", n, n, (time_span3 / time_span1) * (m1 / m3));
	printf("time_span(dptd_fibonacci(%d))/time_span(iterative_fibonacci(%d)):%0.1f\n", n, n, (time_span4 / time_span1) * (m1 / m4));
	printf("time_span(recursive_fibonacci(%d))/time_span(dptd_fibonacci(%d)):%0.1f\n", n, n, (time_span2 / time_span4) * (m4 / m2));

	printf("\n");
}

int main(int argc, char *args[])
{
	if (argc <= 1)
	{
		test_iterative_fibonacci();
		test_recursive_fibonacci();
		test_dpbu_fibonacci();
		test_dptd_fibonacci();
	}
	else
	{
		time_test_fibonacci();
	}
	return 0;
}
