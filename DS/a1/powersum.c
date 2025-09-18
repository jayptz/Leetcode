#include <limits.h>
#include "powersum.h"
int power_overflow(int b, int n){
    if (n < 0) return 1;  // Negative exponent not supported
    if (b == 0) return (n == 0) ? 1 : 0;  // 0^0 is undefined, 0^n = 0
    if (b == 1) return 0;  // 1^n = 1, no overflow
    if (b == -1) return 0; // (-1)^n = ±1, no overflow
    
    int result = 1;
    for (int i = 0; i < n; i++) {
    
        if (b > 0 && result > INT_MAX / b) return 1;
        if (b < 0 && result < INT_MIN / b) return 1;
        result *= b;
    }
    return 0;
}

int mypower(int b, int n){
    if (power_overflow(b, n)) return 0; 
    
    if (n < 0) return 0; 
    if (b == 0) return (n == 0) ? 1 : 0; 
    if (n == 0) return 1; 
    
    int result = 1;
    for (int i = 0; i < n; i++) {
        result *= b;
    }
    return result;
}


int powersum(int b, int n){
    if (n < 0) return 0;  
    
    int sum = 0;
    for (int i = 0; i <= n; i++) {
        int power = mypower(b, i);
        if (power == 0 && i > 0) return 0; 
        
        if (sum > 0 && power > INT_MAX - sum) return 0;
        if (sum < 0 && power < INT_MIN - sum) return 0;
        
        sum += power;
    }
    return sum;
}
