#include "math.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    int result = 0;
    for (int i = 0; i < b; i++) {
        result += a;
    }
    return result;
}

int divide(int a, int b) {
    if (b == 0) return 'Division by zero';

    int count = 0;
    while (a >= b) {
        a -= b;
        count++;
    }
    return count;
}

int mod(int a, int b) {
    if (b == 0) return 0;

    while (a >= b) {
        a -= b;
    }
    return a;
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}