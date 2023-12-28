#include <stdio.h>

double numericalInt(double a, double b, int n, double (*f)(double), int intins) {
    double multval = (b - a) / n;
    double sum = 0;

    for (int i = 0; i < n; i++) {
        sum += f(a + (i + 0.5) * multval);
    }

    sum *= multval;

    return sum;
}

// Example function to integrate (you can replace this with your actual function)
double exampleFunction(double x) {
    return x * x;
}

int main() {
    double a = 0.0;
    double b = 1.0;
    int n = 1000;

    double result = numericalInt(a, b, n, exampleFunction, 0);

    printf("Numerical integral result: %lf\n", result);

    return 0;
}
