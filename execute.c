#include "stdio.h"
#include "filter.h"

/*
File just to execute the circular_goertzel_stream function and find derivative
to get step function for output
*/

int main(void)
{
    double var1;
    int WINDOW_SIZE = 1024;

    FILE *f1 = fopen("output_1kHz.txt", "r");
    int n = 0;
    double x1[10 * WINDOW_SIZE];
    while (fscanf(f1, "%lf", &var1) > 0)
    {
        x1[n] = var1;
        n++;
    }
    fclose(f1);

    FILE *fOO = fopen("circular_goertzel_stream.txt", "w");
    FILE *fDer = fopen("deriv_goertzel_strm.txt", "w");

    double prev_o = 0;
    int hist_dist = 100;
    for (int n = 0; n < 10 * WINDOW_SIZE; n++)
    {
        double o = circular_goertzel_stream(x1[n], 1000, 100000, WINDOW_SIZE);
        if (n == 1023 - hist_dist)
        {
            prev_o = o;
        }
        if (n > 1023)
        {
            if (n % hist_dist == 0)
            {
                int pos = (o - prev_o) > 0 ? 1 : 0;
                fprintf(fDer, "%d\n", pos);
                prev_o = o;
            }
        }
        fprintf(fOO, "%f\n", o);
    }
    fclose(fOO);
    fclose(fDer);
}