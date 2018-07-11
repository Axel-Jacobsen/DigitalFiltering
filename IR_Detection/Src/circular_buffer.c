#include "stdlib.h"
#include "stdio.h"
#include "circular_buffer.h"

/*
Implementation from https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc
*/

int circ_bufsum_init(circ_bufsum_t *cbuf, int window_size)
{
    int r = -1;

    if (cbuf)
    {
        cbuf->head    = 0;
        cbuf->tail    = 0; 
        cbuf->size    = window_size;
        cbuf->num_els = 0;
        cbuf->empty   = 1;
        cbuf->buffer  = (int *)malloc(window_size * sizeof (double));
        r = 1;
    }

    return r;
}

int circ_bufsum_reset(circ_bufsum_t *cbuf)
{
    int r = -1;

    if (cbuf)
    {
        cbuf->head = 0;
        cbuf->tail = 0;
        r = 0;
    }

    return r;
}

/*
Returns emptyness of cbuf - 1 for empty 
*/
int circ_bufsum_empty(circ_bufsum_t *cbuf)
{
    return (cbuf->tail +1) == cbuf->head;
}

int circ_bufsum_full(circ_bufsum_t *cbuf)
{
    return ((cbuf->head + 1) % cbuf->size) == cbuf->tail;
}

int circ_bufsum_put(circ_bufsum_t * cbuf, int data)
{
    int r = -1;

    if (cbuf)
    {
        r = 1;
        cbuf->buffer[cbuf->head] = data;
        cbuf->head = (cbuf->head + 1) % cbuf->size;
        if (cbuf->head == cbuf->tail)
        {
            cbuf->tail = (cbuf->tail + 1) % cbuf->size;
        }
        else
        {
            cbuf->num_els++;
        }
    }

    return r;
}

/*
Add a new sample to the buffer - add it to the sum of the buffer, and remove the value that is overwritten 
*/
double circ_bufsum_add_sample(circ_bufsum_t * cbuf, double data)
{
    double r = -1.0;

    if (cbuf)
    {
        r = 0.0;
        cbuf->buffer[cbuf->head] = data;
        cbuf->head = (cbuf->head + 1) % cbuf->size;

        if (cbuf->head == cbuf->tail)
        {
            r = cbuf->buffer[cbuf->tail];
            cbuf->tail = (cbuf->tail + 1) % cbuf->size;
        }
        else
        {
            cbuf->num_els += 1;
        }
    }

    return r;
}

int inline circ_bufsum_read_head(circ_bufsum_t *cbuf)
{
    if (cbuf)
    {
        return cbuf->buffer[cbuf->head];
    }
    return -1;
}

int inline circ_bufsum_read_tail(circ_bufsum_t *cbuf)
{
    if (cbuf)
    {
        return cbuf->buffer[cbuf->tail];
    }
    return -1;
}
