#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

double calculateSin3(double x, unsigned int n, unsigned int i, double item);
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
    printf("With 3 recursion sin of %lf is %.10lf\n", x, res);
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
    return calculateSin3(x, n, 0, 0);
}

double calculateSin3(double x, unsigned int n, unsigned int i, double item)
{
    double res = 0;
    if (i == n)
    {
        return res;
    }
    else if (i == 0)
    {
        item = x;
    }
    else
    {
        double k;
        k = -1 * x * x / (4 * i * i + 2 * i);
        item *= k;
    }
    res = calculateSin3(x, n, ++i, item);
    res += item;
    return res;
}
