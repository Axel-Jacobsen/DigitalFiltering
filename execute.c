#include "stdio.h"
#include "filter.h"

int main(void)
{
    FILE *f1 = fopen("output_1kHz.txt", "r");
    FILE *f2 = fopen("output_10kHz.txt", "r");
    FILE *f_res = fopen("results1.txt", "w");
    double var1, var2;
    int WINDOW_SIZE = 1024;

    int n = 0;
    double x1[10 * WINDOW_SIZE];
    while (fscanf(f1, "%lf", &var1) > 0)
    {
        x1[n] = var1;
        n++;
    }

    n = 0;
    double x2[10 * WINDOW_SIZE];
    while (fscanf(f2, "%lf", &var2) > 0)
    {
        x2[n] = var2;
        n++;
    }

    FILE *fO1 = fopen("updated_goertzel_1k.txt", "w");
    for (int n = 0; n < 10 * 1024; n++)
    {
        double o = goertzel(x1, 100000, 1000, WINDOW_SIZE);
        fprintf(fO1, "%f\n", o);
    }

    FILE *fO2 = fopen("updated_goertzel_10k.txt", "w");
    for (int n = 0; n < 10 * 1024; n++)
    {
        double o = goertzel(x1, 100000, 1000, WINDOW_SIZE);
        fprintf(fO2, "%f\n", o);
    }

    fclose(f1);
    fclose(f2);
    fclose(fO1);
    fclose(fO2);
    fclose(f_res);

    // FILE *fOO = fopen("circular_goertzel_stream.txt", "w");
    // // Test circular_goertzel_stream
    // for (int n = 0; n < 10 * 1024; n++)
    // {
    //     double o = circular_goertzel_stream(x1[n], 1000, 100000, 1024);
    //     fprintf(fOO, "%f\n", o);
    //     if (o > 0)
    //     {
    //         printf("POWER: %f\n", o);
    //     }
    // }
    // fclose(fOO);
}