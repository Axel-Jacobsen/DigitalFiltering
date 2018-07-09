#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_SIZE 1024
#define PI 3.141592653589793
#define BANDWITH 200
#define NUM_FREQS 2

/*
return the goertzel algorithm evaluation for the given inputs
- x is an array of samples
- sample_rate is the sample rate at which x was taken
- freqs is an array of pairs of lower and upper bounds for frequencies
    - example: freqs = [400, 500, 1000, 1100] would mean that we are looking at 2 bands of frequencies: 400 to 500, and 1000 to 1100
    - by it's nature, freqs must have an even number of inputs
    - BANDWITHS MUST BE THE SAME FOR EACH FREQ PAIR
        - TODO: Generalize for variable bandwiths
- results must be 'results[num_freqs / 2][size_of_bin]'
*/
void goertzel(double *x, int sample_rate, int *freqs, double results[][BANDWITH], double *freqs_out)
{
    // Set up initial parameters
    int window_size = WINDOW_SIZE;
    double f_step = sample_rate / (double)window_size;
    double f_step_normalized = 1.0 / window_size;

    // We need to find the DFT bins that we need to compute
    int num_freqs = NUM_FREQS;
    assert(num_freqs % 2 == 0);
    int bins[num_freqs];
    int f_start, f_end, k_start, k_end;
    for (int i = 0; i < num_freqs; i += 2)
    {
        f_start = freqs[i];
        f_end = freqs[i + 1];
        k_start = (int)floor(f_start / f_step);
        k_end = (int)ceil(f_end / f_step);
        // Make sure frequency is within range- if this throws, your
        // sample rate is too low compared to your highest frequency
        assert(k_end < window_size - 1);
        bins[i] = k_start;
        bins[i + 1] = k_end;
    }

    // number of frequencies is the same size as the number of
    // upper and lower bin limits; loop through bin limits (i.e.
    // k_start and k_end pairs, and evaluate goertzel from there)
    double f, w_real, w_imag;
    // Results will be a 2d array; each row will be an individual frequency band,
    // each frequency band will have k_start - k_end elements.
    // TODO: Generalize for variable bandwiths
    int i = 0;
    for (int j = 0; j < num_freqs; j += 2)
    {
        k_start = bins[j];
        k_end = bins[j + 1];
        // evaluate through the frequencies between k_start and k_end
        for (int k = k_start; k < k_end; k++)
        {
            // Bin frequency and coefficients for computation
            f = k * f_step_normalized;
            w_real = 2.0 * cos(2.0 * PI * f);
            w_imag = sin(2.0 * PI * f);

            double d1, d2, y;
            for (int n = 0; n < window_size; n++)
            {
                y = x[n] + w_real * d1 - d2;
                d2 = d1;
                d1 = y;
            }

            printf("FRQ: %.9f", f * sample_rate);
            printf(" VAL: %.9f\n", d2 * d2 + d1 * d1 - w_real * d1 * d2);
            // Calculate power, and put it in its results spot
            results[j / 2][k - k_start] = d2 * d2 + d1 * d1 - w_real * d1 * d2;
            freqs_out[i] = f * sample_rate;
            i++;
        }
    }
}

int main(void) {
    FILE *f1 = fopen("output1.txt", "r");
    FILE *f2 = fopen("output2.txt", "r");
    FILE *f_res = fopen("results1.txt", "w");
    // FILE *f2 = fopen("output2.txt", "r");
    double var1, var2;
    int n=0;
    double x1[WINDOW_SIZE];
    while(fscanf(f1,"%lf", &var1) > 0){
        x1[n] = var1;
        n++;
    }
    double x2[WINDOW_SIZE];
    n=0;
    while(fscanf(f2,"%lf", &var2) > 0){
        x2[n] = var2;
    }
    double results[NUM_FREQS][BANDWITH];
    double freqs_out[NUM_FREQS * BANDWITH];
    int freqs[NUM_FREQS * 1] = {900, 1100};
    int freqs1[NUM_FREQS * 1] = {9900, 10100};
    goertzel(x1, 100000, freqs, results, freqs_out);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 200; j++) {
            fprintf(f_res, "%.18f,%.18f\n", freqs_out[200 * i + j], results[i][j]);
        }
    }
    double results1[NUM_FREQS][BANDWITH];
    double freqs_out1[NUM_FREQS * BANDWITH];
    goertzel(x2, 100000, freqs1, results1, freqs_out1);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 200; j++) {
            fprintf(f_res, "%.18f,%.18f\n", freqs_out1[200 * i + j], results1[i][j]);
        }
    }
    fclose(f1);
    fclose(f2);
    fclose(f_res);
}