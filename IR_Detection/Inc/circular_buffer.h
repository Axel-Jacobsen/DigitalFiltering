#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include "stdlib.h"

/*
Abstraction Fcn: A circular buffer, i.e. a buffer of fixed length that overwrites old values as new ones are added
*/
typedef struct
{
    int *buffer;
    uint8_t empty;
    size_t num_els;
    size_t head;
    size_t tail;
    size_t size;
} circ_bufsum_t;

int circ_bufsum_init(circ_bufsum_t *cbuf, int window_size);
int circ_bufsum_reset(circ_bufsum_t *cbuf);
int circ_bufsum_empty(circ_bufsum_t *cbuf);
int circ_bufsum_full(circ_bufsum_t *cbuf);
int circ_bufsum_put(circ_bufsum_t *cbuf, int data);
double circ_bufsum_add_sample(circ_bufsum_t *cbuf, double data);
int inline circ_bufsum_read_head(circ_bufsum_t *cbuf);
int inline circ_bufsum_read_tail(circ_bufsum_t *cbuf);

#endif