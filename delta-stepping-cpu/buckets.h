//
//  buckets.h
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#ifndef buckets_h
#define buckets_h

#include "constants.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct Buckets {

    unsigned int *memory;
    unsigned int **bucket_position;
    
    unsigned int *head;
    unsigned int *tail;
    
    unsigned int *cur_bucket;
    unsigned int cur_bucket_num;
    
    unsigned int delta;
    unsigned int num_buckets;

} Buckets;

void buckets_create(unsigned int num_vertices, unsigned int max_edge, unsigned int delta, Buckets *buckets);

void buckets_add_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex);

void buckets_clear_bucket(Buckets *buckets, unsigned int bucket_number);

void buckets_assign_next_bucket(Buckets *buckets, unsigned int current_bucket_number);

void buckets_remove_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex);

unsigned int buckets_is_bucket_empty(Buckets *buckets, unsigned int bucket_number);

unsigned int buckets_is_buckets_empty(Buckets *buckets);

unsigned int buckets_pos_of_vertex_in_bucket(Buckets buckets, unsigned int bucket_number, unsigned int vertex);

void buckets_remove_vertex_pos_in_bucket(Buckets *buckets, unsigned int bucket_number, unsigned int pos);

void print_bucket(Buckets buckets);

#endif /* buckets_h */
