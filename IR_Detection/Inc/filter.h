/*
* HEADER FILE FOR filter.c
* Not sure what to include, so eskettit
*/

#ifndef FILTER_H
#define FILTER_H

#define PI 3.141592653589793

// Change these for goertzel() ONLY!!!!
#define BANDWITH 200
#define NUM_FREQS 2

double goertzel(int *x, int sample_rate, int freq, int window_size);
double tandem_goertzel_stream(double x, int freq, int samplefreq, int resetsample);
double circular_goertzel_stream(double x, int freq, int sample_rate, int window_size);

#endif
