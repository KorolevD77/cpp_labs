#include "my_math.hpp"

double factorial(int n) {
    double rez = 1.0;
    for (int i = 2; i <= n; ++i) 
        rez *= i;
    return rez;
}

double abs(double x){
    return (x < 0 ? (-x) : x);
}

double power(double base, int exp) {
    double rez = 1.0;
    for (int i = 0; i < exp; ++i)
        rez *= base;
    return rez;
}

double norm_angle(double x) {    
    while(x > PI) x -= PI;
    while(x < -PI) x += PI;
    return x;
}

AnswerHandler my_sin(double x) {    
    x = norm_angle(x);
    double rez = 0.0;
    double term = x;
    int n = 1;
    
    for (int i = 0; i < MAX_ITS_FOR_MY_SIN; ++i) {
        rez += term;
        n += 2;
        term = -term * x * x / ((n - 1) * n);
        if (abs(term) < EPSILON_FOR_MY_SIN) break;
    }
    
    return AnswerHandler{x, rez};
}