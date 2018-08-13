#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "filter.h"
#include "circular_buffer.h"

#define PI 3.141592653589793

double goertzel(double *x, int sample_rate, int freq, int window_size)
{
    // Set up initial parameters
    double f_step = sample_rate / (double)window_size;
    double f_step_normalized = 1.0 / window_size;

    double k = freq / f_step;

    // number of frequencies is the same size as the number of
    // upper and lower bin limits; loop through bin limits (i.e.
    // k_start and k_end pairs, and evaluate goertzel from there)
    double normalizedfreq, w_real, w_imag;

    normalizedfreq = k * f_step_normalized;
    w_real = 2.0 * cos(2.0 * PI * normalizedfreq);

    double d1 = 0, d2 = 0, y = 0;
    for (int n = 0; n < window_size; n++)
    {
        y = x[n] + w_real * d1 - d2;
        d2 = d1;
        d1 = y;
    }

    // Calculate power, and put it in its results spot
    return d2 * d2 + d1 * d1 - w_real * d1 * d2;
}

double circular_goertzel_stream(double x, int freq, int sample_rate, int window_size)
{
    // Set up and initialize circular_buffer
    static int initialized = 0;
    static circ_bufsum_t cbuf;
    if (!initialized)
    {
        circ_bufsum_init(&cbuf, window_size);
        initialized = 1;
    }

    // Set up initial parameters
    double f_step = sample_rate / (double)window_size;
    double f_step_normalized = 1.0 / window_size;

    // Calculate freq bin (for calc of frequency)
    double k = freq / f_step;

    // Get rid of for loop of num_freqs,etc
    // Bin frequency and coefficients for computation
    double normalizedfreq, w_real;
    normalizedfreq = k * f_step_normalized;
    w_real = 2.0 * cos(2.0 * PI * normalizedfreq);

    /*
    ALGO:
        while cbuf->size < WINDOW_SIZE
            perform this loop (put new values of y into cbuf)
        put new values into cbuf, subtract old y from d1 values
    */
    static double d1 = 0;
    static double d2 = 0; 
    static double y  = 0;
    static double total_power = 0;
    // Now the cbuf is full; add values, and subtract old y from d1
    double a = circ_bufsum_add_sample(&cbuf, y);
    y = x + w_real * d1 - d2;
    d2 = d1;
    d1 = y;

    // The issue here is trying to figure out how to subtract previous 
    // values from the current output y to maintain a plateau when 
    // measuring the desired frequency consistently. This can be done when 
    // the inputs x follow a specific function, but not when they are real samples 
    // (as far as I understand). If anyone knows better, let me know! For now, 
    // nothing happens.

    // Calculate power, and put it in its results spot
    total_power += x*x;
    return ((d2 * d2 + d1 * d1 - w_real * d1 * d2) / total_power / cbuf.num_els);
}

/*
Takes a stream of samples at sample_rate. returns current power every iteration
sample_rate = 1024
resetsample = 512

Adapted from https://netwerkt.wordpress.com/2011/08/25/goertzel-filter/
*/
double tandem_goertzel_stream(double x, int freq, int sample_rate, int resetsample)
{
    static double s_prev[2] = {0.0, 0.0};
    static double s_prev2[2] = {0.0, 0.0};
    static double total_power[2] = {0.0, 0.0};
    static int N = 0;
    double w_real, normalizedfreq, power, s;
    int active;
    static int n[2] = {0, 0};

    normalizedfreq = freq / sample_rate;
    w_real = 2.0 * cos(2.0 * PI * normalizedfreq);

    // Calculate the first set of samples
    s = x + w_real * s_prev[0] - s_prev2[0];
    s_prev2[0] = s_prev[0];
    s_prev[0] = s;
    n[0]++;

    // Calculate the second set of samples
    s = x + w_real * s_prev[1] - s_prev2[1];
    s_prev2[1] = s_prev[1];
    s_prev[1] = s;
    n[1]++;

    N++;

    // Reset inactive
    active = (N / resetsample) & 0x01;
    if (n[1 - active] >= resetsample)
    {
        s_prev[1 - active] = 0.0;
        s_prev2[1 - active] = 0.0;
        total_power[1 - active] = 0.0;
        n[1 - active] = 0;
    }
    total_power[0] += x * x;
    total_power[1] += x * x;
    power = s_prev2[active] * s_prev2[active] + s_prev[active] * s_prev[active] - w_real * s_prev[active] * s_prev2[active];

    return power / (total_power[active] + 1e-7) / n[active];
}
