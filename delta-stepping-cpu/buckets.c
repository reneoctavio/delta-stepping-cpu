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
    SingleNode **b_mem = malloc(sizeof(SingleNode*) * num_of_buckets);
    
    //
    for (unsigned int i = 0; i < num_of_buckets; i++) {
        b_mem[i] = NULL;
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
    /*
    if (bucket_number < buckets->cur_bucket_num) {
        printf("Add: Target#: %u, Current#: %u\n", bucket_number, buckets->cur_bucket_num);
    }
    if (bucket_number >= (buckets->cur_bucket_num + buckets->num_buckets)) {
        printf("Add: Target#: %u, Last#: %u\n", bucket_number, (buckets->cur_bucket_num + buckets->num_buckets - 1));
    }*/
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets);
    SingleNode **bucket = &buckets->list[bucket_pos];
    
    push(bucket, vertex);
}

void buckets_clear_bucket(Buckets *buckets, unsigned int bucket_number) {
    // Get bucket position
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets);
    SingleNode **bucket = &buckets->list[bucket_pos];
    
    // Clear bucket
    unsigned int i = 0;
    while (i != UINT_MAX) {
        i = pop(bucket);
    }
    
}

void buckets_remove_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex) {
    // Get bucket position
    if (bucket_number < buckets->cur_bucket_num) {
        printf("Rem: Target#: %u, Current#: %u\n", bucket_number, buckets->cur_bucket_num);
    }
    if (bucket_number >= (buckets->cur_bucket_num + buckets->num_buckets)) {
        printf("Rem: Target#: %u, Last#: %u\n", bucket_number, (buckets->cur_bucket_num + buckets->num_buckets - 1));
    }
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets);
    SingleNode **bucket = &buckets->list[bucket_pos];
    
    remove_by_value(bucket, vertex);
}

unsigned int buckets_is_bucket_empty(Buckets *buckets, unsigned int bucket_number) {
    unsigned int bucket_pos = (bucket_number % buckets->num_buckets);
    SingleNode *bucket = buckets->list[bucket_pos];
    
    if (bucket == NULL) {
        return 1;
    }
    return 0;
}

unsigned int buckets_is_buckets_empty(Buckets *buckets) {
    for (unsigned int i = 0; i < buckets->num_buckets; i++) {
        if (!buckets_is_bucket_empty(buckets, i)) {
            return 0;
        }
    }
    return 1;
}

void buckets_free(Buckets *buckets) {
    for (unsigned int i = 0; i < buckets->num_buckets; i++) {
        buckets_clear_bucket(buckets, i);
    }
    free(buckets->list);
}

void print_bucket(Buckets buckets) {
    printf("Buckets: \n");
    for (unsigned int i = buckets.cur_bucket_num; i < (buckets.num_buckets + buckets.cur_bucket_num); i++) {
        printf("Bucket #%02u: ", i);
        // Get bucket position
        unsigned int bucket_pos = (i % buckets.num_buckets);
        SingleNode *bucket = buckets.list[bucket_pos];

        while (bucket != NULL) {
            unsigned int vertex = bucket->value;
            if (vertex == UINT_MAX) {
                printf(" -- ");
            }
            else {
                printf(" %02u ", vertex);
            }
            bucket = bucket->next;
        }
        printf("\n");
    }
}