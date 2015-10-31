//
//  vector.c
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/31/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#include "vector.h"

void vector_init(Vector *vector, unsigned int initial_capacity) {
    // initialize size and capacity
    vector->size = 0;
    vector->capacity = initial_capacity;
    
    // allocate memory for vector->data
    vector->data = malloc(sizeof(unsigned int) * vector->capacity);
}

void vector_append(Vector *vector, unsigned int value) {
    // make sure there's room to expand into
    vector_double_capacity_if_full(vector);
    
    // append the value and increment vector->size
    vector->data[vector->size++] = value;
}

void vector_remove_last(Vector *vector, unsigned int *value) {
    // Get the final value and decrease size
    *value = vector->data[vector->size--];
}

void vector_clear(Vector *vector) {
    // Remove all values
    vector->size = 0;
}

int vector_get(Vector *vector, unsigned int index) {
    if (index >= vector->size) {
        printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
        exit(1);
    }
    return vector->data[index];
}

void vector_set(Vector *vector, unsigned int index, unsigned int value) {
    // zero fill the vector up to the desired index
    while (index >= vector->size) {
        vector_append(vector, 0);
    }
    
    // set the value at the desired index
    vector->data[index] = value;
}

void vector_double_capacity_if_full(Vector *vector) {
    if (vector->size >= vector->capacity) {
        // double vector->capacity and resize the allocated memory accordingly
        vector->capacity *= 2;
        vector->data = realloc(vector->data, sizeof(unsigned int) * vector->capacity);
    }
}

void vector_free(Vector *vector) {
    free(vector->data);
}