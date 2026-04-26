#include "math.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    int result = 0;
    int abs_a = abs_val(a);
    int abs_b = abs_val(b);
    for (int i = 0; i < abs_b; i++) {
        result += abs_a;
    }
    if ((a < 0 && b > 0) || (a > 0 && b < 0)) {
        return -result;
    }
    return result;
}

int divide(int a, int b) {
    if (b == 0) return 0;

    int abs_a = abs_val(a);
    int abs_b = abs_val(b);
    int count = 0;
    
    while (abs_a >= abs_b) {
        abs_a -= abs_b;
        count++;
    }
    
    if ((a < 0 && b > 0) || (a > 0 && b < 0)) {
        return -count;
    }
    return count;
}

int mod(int a, int b) {
    if (b == 0) return 0;

    int abs_a = abs_val(a);
    int abs_b = abs_val(b);

    while (abs_a >= abs_b) {
        abs_a -= abs_b;
    }
    
    return a < 0 ? -abs_a : abs_a;
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}