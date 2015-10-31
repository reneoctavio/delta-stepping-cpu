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
    
    // Buckets hold no values
    for (unsigned int i = 0; i < (num_of_buckets * delta); i++) {
        b_mem[i] = UINT_MAX;
    }
    
    // Copy out
    buckets->memory = b_mem;
    
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
        unsigned int pos = buckets_get_pos_in_memory(*buckets, bucket_number, i);
        if (buckets->memory[pos] == UINT_MAX) {
            buckets->memory[pos] = vertex;
            break;
        }
    }
}

void buckets_clear_bucket(Buckets *buckets, unsigned int bucket_number) {
    for (unsigned int i = 0; i < buckets->delta; i++) {
        unsigned int pos = buckets_get_pos_in_memory(*buckets, bucket_number, i);
        buckets->memory[pos] = UINT_MAX;
    }
}

void buckets_remove_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex) {
    for (unsigned int i = 0; i < buckets->delta; i++) {
        unsigned int pos = buckets_get_pos_in_memory(*buckets, bucket_number, i);
        if (buckets->memory[pos] == vertex) {
            buckets->memory[pos] = UINT_MAX;
            break;
        }
    }
}

void buckets_remove_vertex_pos_in_bucket(Buckets *buckets, unsigned int bucket_number, unsigned int pos) {
    unsigned int pos_mem = buckets_pos_of_vertex_in_bucket(*buckets, bucket_number, pos);
    buckets->memory[pos_mem] = UINT_MAX;
}

unsigned int buckets_is_bucket_empty(Buckets *buckets, unsigned int bucket_number) {
    for (unsigned int i = 0; i < buckets->delta; i++) {
        unsigned int pos = buckets_get_pos_in_memory(*buckets, bucket_number, i);
        if (buckets->memory[pos] != UINT_MAX) {
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
        unsigned int pos = buckets_get_pos_in_memory(buckets, bucket_number, i);
        if (buckets.memory[pos] == vertex) {
            return i;
        }
    }
    return UINT_MAX;
}

void buckets_free(Buckets *buckets) {
    free(buckets->memory);
}

void print_bucket(Buckets buckets) {
    printf("Buckets: \n");
    for (unsigned int i = buckets.cur_bucket_num; i < (buckets.num_buckets + buckets.cur_bucket_num); i++) {
        printf("Bucket #%02u: ", i);
        for (unsigned int j = 0; j < buckets.delta; j++) {
            unsigned int pos = buckets_get_pos_in_memory(buckets, i, j);
            if (buckets.memory[pos] == UINT_MAX) {
                printf(" -- ");
            }
            else {
                printf(" %02u ", buckets.memory[pos]);
            }
        }
        printf("\n");
    }
}

unsigned int buckets_get_pos_in_memory(Buckets buckets, unsigned int cur_bucket, unsigned int pos_in_bucket) {
    return (cur_bucket % buckets.num_buckets) * buckets.delta + pos_in_bucket;
}