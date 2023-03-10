#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

double calculateSin1(double x, unsigned int n, unsigned int i, double item, double s);
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
    printf("With 1 recursion sin of %lf is %.10lf\n", x, res);
}

double wrappedSin(double x, unsigned int n)
{
    if (n == 0)
    {
        printf("N must be greater than 0");
    }
    //Usage of periodic property to calculate sin(x) correctly with any x
    int p = x / (2. * M_PI);
    x -= 2. * M_PI * p;
    return calculateSin1(x, n, 1, x, x);
}

double calculateSin1(double x, unsigned int n, unsigned int i, double item, double s)
{
    double res = 0;
    double k;
    if (i == n)
    {
        res = s;
        return res;
    }
    k = -1 * x * x / (4 * i * i + 2 * i);
    item *= k;
    s += item;
    res = calculateSin1(x, n, ++i, item, s);
    return res;
}
