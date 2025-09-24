/**
 * your program signature
 */ 

#include "fibonacci.h"

 /**
  * int recursive_fibonacci(int n) which computes and returns the nth Fibonacci number F(n) using recursive algorithm, namely using recursion function. It returns -1 if overflow occurs.
  */
 int recursive_fibonacci(int n) {
    if (n <= 0){
        return 0;
    }else if (n == 1){
        return 1;
    }else{
        int a = recursive_fibonacci(n - 1);
        if (a == -1){
            return -1;
        }
        int b = recursive_fibonacci(n - 2);
        if (b == -1){
            return -1;
        }
        if (a > 2147483647 - b){
            return -1;
        }
        return a + b;
    }

 }
 
 /**
  *    int iterative_fibonacci(int n) which computes and returns the nth Fibonacci number F(n), using iterative algorithm, namely using a for or while loop. It returns -1 if overflow occurs.
  */
 int iterative_fibonacci(int n){
    if (n < 0){
        return -1;
    }
    if (n == 0){
        return 0;
    }
    if (n == 1){
        return 1;
    }

    int f1 = 0;
    int f2 = 1;
    for (int i = 2; i <= n; i++){
        if (f2 > 2147483647 - f1){
            return -1;
        }
        int temp = f1 + f2;
        f1 = f2;
        f2 = temp;
    }
    return f2;
 }
 
 /**
  * int dpbu_fibonacci(int *f, int n) which computes and returns the nth Fibonacci number F(n) using using dynamic programming bottom-up method with external array f[n+1] of initial value 0 for all elements. The algorithm fills up f[] by f[0]=0, f[1]=1, f[i]= f[i-2]+a[i-1] for i=2,...,n, and return f[n]. It returns -1 if overflow occurs.
  */
 int dpbu_fibonacci(int *f, int n) {
   if (n < 0){
       return -1;
   }
   if (n == 0){
       if (f){
           f[0] = 0;
       }
       return 0;
   }
   if (n == 1){
       if (f){
           f[0] = 0;
           f[1] = 1;
       }
       return 1;
   }

   f[0] = 0;
   f[1] = 1;
   for (int i = 2; i <= n; i++){
      if (f[i - 1] > 2147483647 - f[i - 2]){
          return -1;
      }
      f[i] = f[i - 1] + f[i - 2];
   }
   return f[n];
 }
 
 /**
  * int dptd_fibonacci(int *f, int n) which computes and returns the Fibonacci number F(n) using dynamic programming top-down method with external array f[n+1] of initial value 0 for all elements. It fills up f[n+1] by recursion function call. Specifically it returns f[n] if f[n]>0 else sets f[n] = dptd_fibonacci(f, n-2) + dptd_fibonacci(f, n-1) and then returns f[n]. It returns -1 if overflow occurs. 
  */
 int dptd_fibonacci(int *f, int n) {
   if (n <= 0){
       return 0;
   }
   if (n == 1){
       return 1;
   }
   if (f[n] > 0){
       return f[n];
   }

  int a = dptd_fibonacci(f, n - 1);
  if (a == -1){
      return -1;
  }
  int b = dptd_fibonacci(f, n - 2);
  if (b == -1){
      return -1;
  }
  if (a > 2147483647 - b){
      return -1;
  }
  f[n] = a + b;
  return f[n];
 }