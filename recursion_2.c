#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

double calculateSin2(double x, unsigned int n, double *sum);
double wrappedSin(double x, unsigned int n);

int main()
{
    double x;
    unsigned int n;
    printf("Input your X: ");
    scanf_s("%lf", &x);
    printf("Input your N: ");
    scanf_s("%u", &n);

    double res = wrappedSin(x, n);
    printf("With 2 recursion sin of %lf is %.10lf\n", x, res);
}

double wrappedSin(double x, unsigned int n)
{
    double sum = 0;
    if (n == 0)
    {
        printf("N must be greater than 0");
    }
    //Usage of periodic property to calculate sin(x) correctly with any x
    int p = x / (2. * M_PI);
    x -= 2. * M_PI * p;
    calculateSin2(x, n - 1, &sum);
    return sum;
}

double calculateSin2(double x, unsigned int n, double *sum)
{
    double item = 0;
    if (n == 0)
    {
        item = x;
    }
    else
    {
        double k = -1 * x * x / (4 * n * n + 2 * n);
        item = k * calculateSin2(x, n - 1, sum);
    }
    *sum += item;
    return item;
}
