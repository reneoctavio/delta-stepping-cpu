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
    
    // Allocate buckets
    Vector *b_mem = malloc(sizeof(Vector) * num_of_buckets);
    
    // Allocate a dynamic vetor to each bucket, initial size of delta
    for (unsigned int i = 0; i < num_of_buckets; i++) {
        vector_init(&b_mem[i], delta);
    }
    
    // Copy out
    buckets->list = b_mem;
    
    // Props
    buckets->delta = delta;
    buckets->num_buckets = num_of_buckets;
    buckets->cur_bucket_num = 0;
}

void buckets_add_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex) {
    // Get bucket position
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets) * buckets->delta;
    Vector *bucket = &buckets->list[bucket_pos];
    
    // If there is an empty place, put it in there
    for (unsigned int i = 0; i < bucket->size; i++) {
        if (bucket->data[i] == UINT_MAX) {
            bucket->data[i] = vertex;
            return;
        }
    }
    
    // If not, append
    vector_append(bucket, vertex);
}

void buckets_clear_bucket(Buckets *buckets, unsigned int bucket_number) {
    // Get bucket position
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets) * buckets->delta;
    Vector *bucket = &buckets->list[bucket_pos];
    
    // Clear vector
    vector_clear(bucket);
}

void buckets_remove_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex) {
    // Get bucket position
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets) * buckets->delta;
    Vector *bucket = &buckets->list[bucket_pos];
    
    if (bucket->size == 0) {
        return;
    }
    
    for (unsigned int i = 0; i < bucket->size; i++) {
        if (bucket->data[i] == vertex) {
            if (i == (bucket->size - 1)) {
                bucket->size--;
                return;
            }
            bucket->data[i] = UINT_MAX;
            return;
        }
    }
}

unsigned int buckets_is_bucket_empty(Buckets *buckets, unsigned int bucket_number) {
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets) * buckets->delta;
    if (buckets->list[bucket_pos].size == 0) {
        return 1;
    }
    return 0;
}

unsigned int buckets_is_buckets_empty(Buckets *buckets) {
    for (unsigned int i = 0; i < (buckets->num_buckets * buckets->delta); i++) {
        if (buckets->list[i].size != 0) {
            return 0;
        }
    }
    return 1;
}

unsigned int buckets_get_vertex(Buckets buckets, unsigned int cur_bucket, unsigned int pos_in_bucket) {
    // Get bucket position
    unsigned int bucket_pos = (cur_bucket % buckets.num_buckets) * buckets.delta;
    // Get vertex from a position inside the selected bucket
    Vector *bucket = &buckets.list[bucket_pos];
    if (pos_in_bucket > bucket->size) {
        return UINT_MAX;
    }
    return bucket->data[pos_in_bucket];
}

void buckets_free(Buckets *buckets) {
    for (unsigned int i = 0; i < buckets->num_buckets; i++) {
        Vector *temp = &buckets->list[i];
        vector_free(temp);
    }
    free(buckets->list);
}

void print_bucket(Buckets buckets) {
    printf("Buckets: \n");
    for (unsigned int i = buckets.cur_bucket_num; i < (buckets.num_buckets + buckets.cur_bucket_num); i++) {
        printf("Bucket #%02u: ", i);
        // Get bucket position
        unsigned int bucket_pos = (i % buckets.num_buckets) * buckets.delta;
        Vector *bucket = &buckets.list[bucket_pos];
        
        for (unsigned int j = 0; j < bucket->size; j++) {
            unsigned int vertex = bucket->data[j];
            if (vertex == UINT_MAX) {
                printf(" -- ");
            }
            else {
                printf(" %02u ", vertex);
            }
        }
        printf("\n");
    }
}