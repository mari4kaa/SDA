//This function helps to print all necessary data for diagram

void test()
{
    for (double x = -30.; x < 30.; x += 0.1)
    {
        printf("%lf\t", x);
        double y = sin(x);
        for (int n = 3; n < 21; n += 3)
        {
            double res = wrappedSin(x, n);
            double diff = fabs(y - res);
            printf("%lf\t", diff);
        }
        printf("\n");
    }
}
