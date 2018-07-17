#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "filter.h"
#include "circular_buffer.h"

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

TODO:
    - optimize w/ pointers
    - optimize for use in other files
*/

double goertzel(int *x, int sample_rate, int freq, int window_size)
{
    // Set up initial parameters
    double f_step = sample_rate / (double)window_size;
    double f_step_normalized = 1.0 / window_size;

    uint32_t k = (uint32_t)floor(freq / f_step);

    // number of frequencies is the same size as the number of
    // upper and lower bin limits; loop through bin limits (i.e.
    // k_start and k_end pairs, and evaluate goertzel from there)
    double normalizedfreq, w_real;

    normalizedfreq = k * f_step_normalized;
    w_real = 2.0 * cos(2.0 * PI * normalizedfreq);
    double d1 = 0, d2 = 0, y = 0, tot_power = 0;
    int n = 0;
    while (n < window_size)
    {
        y = x[n] + w_real * d1 - d2;
        d2 = d1;
        d1 = y;
    }

    // Calculate power, and put it in its results spot
    return d2 * d2 + d1 * d1 - w_real * d1 * d2 / tot_power;
}

double circular_goertzel_stream(double x, int freq, int sample_rate, int window_size)
{
    // // Set up and initialize circular_buffer
    // static int initialized = 0;
    // static circ_bufsum_t cbuf;
    // if (!initialized)
    // {
    //     circ_bufsum_init(&cbuf, window_size);
    //     initialized = 1;
    // }

    // Set up initial parameters
    double f_step = sample_rate / (double)window_size;
    double f_step_normalized = 1.0 / window_size;

    // Calculate freq bin (for calc of frequency)
    double k = freq / f_step;

    // Get rid of for loop of num_freqs,etc
    // Bin frequency and coefficients for computation
    // TODO: MAKE STATIC? i.e. w_real is constant across t
    //       for one frequency!
    double normalizedfreq, w_real;
    normalizedfreq = k * f_step_normalized;
    w_real = 2.0 * cos(2.0 * PI * normalizedfreq);

    /*
    ALGO:
        while cbuf->size < WINDOW_SIZE
            perform this loop (put new values of y into cbuf)
        put new values into cbuf, subtract old y from d1 values

    TODO: Optimize circ_bufsum_add_sample to be able to be used when queue is not empty
    */
    static double d1 = 0;
    static double d2 = 0;
    static double y = 0;
    static double total_power = 0;
    // Now the cbuf is full; add values, and subtract old y from d1
    // double a = circ_bufsum_add_sample(&cbuf, y);
    y = x + w_real * d1 - d2;
    d2 = d1;
    d1 = y;

    // printf("FRQ: %.9f", normalizedfreq * sample_rate);
    // printf(" VAL: %.9f\n", d2 * d2 + d1 * d1 - w_real * d1 * d2);
    // // Calculate power, and put it in its results spot
    // MAY NEED TO NORMAILZE POWER
    return log((d2 * d2 + d1 * d1 - w_real * d1 * d2));
}
    // double circular_goertzel_stream(double x, int freq, int sample_rate, int window_size)
    // {
    //     // Set up and initialize circular_buffer
    //     // static int initialized = 0;
    //     // static circ_bufsum_t cbuf;
    //     // if (!initialized)
    //     // {
    //     //     circ_bufsum_init(&cbuf, window_size);
    //     //     initialized = 1;
    //     // }

    //     // Set up initial parameters
    //     double f_step = sample_rate / (double)window_size;
    //     double f_step_normalized = 1.0 / window_size;

    //     // Calculate freq bin (for calc of frequency)
    //     uint32_t k = (uint32_t)floor(freq / f_step);

    //     // Get rid of for loop of num_freqs,etc
    //     // Bin frequency and coefficients for computation
    //     // TODO: MAKE STATIC? i.e. w_real is constant across t
    //     //       for one frequency!
    //     static double normalizedfreq, w_real;
    //     normalizedfreq = k * f_step_normalized;
    //     w_real = 2.0 * cos(2.0 * PI * normalizedfreq);

    //     /*
    //     ALGO:
    //         while cbuf->size < WINDOW_SIZE
    //             perform this loop (put new values of y into cbuf)
    //         put new values into cbuf, subtract old y from d1 values
    //     */
    //     static double d1, d2, y, total_power;
    //     // Now the cbuf is full; add values, and subtract old y from d1
    //     // double a = circ_bufsum_add_sample(&cbuf, y);
    //     y = x + w_real * d1 - d2;
    //     d2 = d1;
    //     d1 = y;

    //     // printf("FRQ: %.9f", normalizedfreq * sample_rate);
    //     // printf(" VAL: %.9f\n", d2 * d2 + d1 * d1 - w_real * d1 * d2);
    //     // // Calculate power, and put it in its results spot
    //     // MAY NEED TO NORMAILZE POWER
    //     total_power += x * x;
    //     return ((d2 * d2 + d1 * d1 - w_real * d1 * d2) / total_power);
    // }

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

        // reset inactive
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

        // printf("\n\npoooower %f\n", power);
        return power / (total_power[active] + 1e-7) / n[active];
    }
