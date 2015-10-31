//
//  vector.h
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/31/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#ifndef vector_h
#define vector_h

#include <stdio.h>
#include <stdlib.h>

// Define a vector type
typedef struct {
    unsigned int size;      // slots used so far
    unsigned int capacity;  // total available slots
    unsigned int *data;     // array of integers we're storing
} Vector;

void vector_init(Vector *vector, unsigned int initial_capacity);

void vector_append(Vector *vector, unsigned int value);

void vector_remove_last(Vector *vector, unsigned int *value);

void vector_clear(Vector *vector);

int vector_get(Vector *vector, unsigned int index);

void vector_set(Vector *vector, unsigned int index, unsigned int value);

void vector_double_capacity_if_full(Vector *vector);

void vector_free(Vector *vector);

#endif /* vector_h */
