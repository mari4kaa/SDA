#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

double calculateSinLoop(double x, unsigned int n);

int main()
{
    double x;
    unsigned int n;
    printf("Input your X: ");
    scanf_s("%lf", &x);
    printf("Input your N: ");
    scanf_s("%u", &n);

    //Usage of periodic property to calculate sin(x) correctly with any x
    int p = x / (2. * M_PI);
    x -= 2. * M_PI * p;
    double res = calculateSinLoop(x, n);
    printf("With loop sin of %lf is %.10lf\n", x, res);
}

double calculateSinLoop(double x, unsigned int n)
{
    double sum = x;
    double item = x;
    for (unsigned int i = 1; i < n; i++)
    {
        double k = -1 * x * x / (4 * i * i + 2 * i);
        item *= k;
        sum += item;
    }
    return sum;
}
