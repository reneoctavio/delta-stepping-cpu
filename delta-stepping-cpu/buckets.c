//
//  buckets.c
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#include "buckets.h"

void buckets_create(unsigned int num_vertices, unsigned int max_edge, unsigned int delta, Buckets *buckets) {
    
    // Calculate number size of memory for buckets
    unsigned int num_of_buckets = ceil((double) max_edge / (double) delta);
    
    // Allocate memory for buckets
    unsigned int *b_mem = malloc(sizeof(unsigned int) * num_of_buckets * delta);
    
    // Allocate bucket position array
    unsigned int **b_pos = malloc(sizeof(unsigned int *) * (max_edge + 1));
    
    // Buckets hold no values
    for (unsigned int i = 0; i < (num_of_buckets * delta); i++) {
        b_mem[i] = UINT_MAX;
    }
    
    // Point to nothing
    for (unsigned int i = 0; i <= max_edge; i++) {
        b_pos[i] = NULL;
    }
    
    // Assign first pointers
    for (unsigned int i = 0; i < num_of_buckets; i++) {
        b_pos[i] = b_mem + (i * delta);
    }
    
    // Copy out
    buckets->memory = b_mem;
    buckets->bucket_position = b_pos;
    
    // Set common positions in memory
    buckets->head = b_mem;
    buckets->tail = b_mem + (num_of_buckets * delta) - 1;
    
    // Props
    buckets->delta = delta;
    buckets->num_buckets = num_of_buckets;
    
    // Current bucket
    buckets->cur_bucket = buckets->head;
    buckets->cur_bucket_num = 0;
}

void buckets_add_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex) {
    for (unsigned int i = 0; i < buckets->delta; i++) {
        if (*(buckets->bucket_position[bucket_number] + i) == UINT_MAX) {
            *(buckets->bucket_position[bucket_number] + i) = vertex;
            break;
        }
    }
}

void buckets_clear_bucket(Buckets *buckets, unsigned int bucket_number) {
    for (unsigned int i = 0; i < buckets->delta; i++) {
        *(buckets->bucket_position[bucket_number] + i) = UINT_MAX;
    }
}

void buckets_assign_next_bucket(Buckets *buckets, unsigned int current_bucket_number) {
    
    // Clear current
    buckets_clear_bucket(buckets, current_bucket_number);
    
    // Current is next
    buckets->cur_bucket = buckets->cur_bucket + buckets->delta;
    // Wrap around if array is finished
    if (buckets->cur_bucket > buckets->tail) {
        buckets->cur_bucket = buckets->head;
    }
    buckets->cur_bucket_num += 1;
    
    // Give room for newer buckets, remove reference from next
    buckets->bucket_position[current_bucket_number + buckets->num_buckets] = buckets->bucket_position[current_bucket_number];
    buckets->bucket_position[current_bucket_number] =  NULL;
}

void buckets_remove_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex) {
    for (unsigned int i = 0; i < buckets->delta; i++) {
        if (*(buckets->bucket_position[bucket_number] + i) == vertex) {
            *(buckets->bucket_position[bucket_number] + i) = UINT_MAX;
            break;
        }
    }
}

void buckets_remove_vertex_pos_in_bucket(Buckets *buckets, unsigned int bucket_number, unsigned int pos) {
    *(buckets->bucket_position[bucket_number + pos]) = UINT_MAX;
}

unsigned int buckets_is_bucket_empty(Buckets *buckets, unsigned int bucket_number) {
    for (unsigned int i = 0; i < buckets->delta; i++) {
        if (*(buckets->bucket_position[bucket_number] + i) != UINT_MAX) {
            return 0;
        }
    }
    return 1;
}

unsigned int buckets_is_buckets_empty(Buckets *buckets) {
    for (unsigned int i = 0; i < (buckets->num_buckets * buckets->delta); i++) {
        if (buckets->memory[i] != UINT_MAX) {
            return 0;
        }
    }
    return 1;
}

unsigned int buckets_pos_of_vertex_in_bucket(Buckets buckets, unsigned int bucket_number, unsigned int vertex) {
    for (unsigned int i = 0; i < buckets.delta; i++) {
        if (*(buckets.bucket_position[bucket_number] + i) == vertex) {
            return i;
        }
    }
    return UINT_MAX;
}

void buckets_free(Buckets *buckets) {
    free(buckets->memory);
    free(buckets->bucket_position);
}

void print_bucket(Buckets buckets) {
    printf("Buckets: \n");
    for (unsigned int i = buckets.cur_bucket_num; i < (buckets.num_buckets + buckets.cur_bucket_num); i++) {
        printf("Bucket #%02u: ", i);
        for (unsigned int j = 0; j < buckets.delta; j++) {
            if (*(buckets.bucket_position[i] + j) == UINT_MAX) {
                printf(" -- ");
            }
            else {
                printf(" %02u ", *(buckets.bucket_position[i] + j));
            }
        }
        printf("\n");
    }
}