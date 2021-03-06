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
#include "lists.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct Buckets {

    SingleNode **list;
    
    unsigned int cur_bucket_num;
    
    unsigned int delta;
    unsigned int num_buckets;

} Buckets;

void buckets_create(unsigned int num_vertices, unsigned int max_edge, unsigned int delta, Buckets *buckets);

void buckets_add_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex);

void buckets_clear_bucket(Buckets *buckets, unsigned int bucket_number);

void buckets_remove_vertex(Buckets *buckets, unsigned int bucket_number, unsigned int vertex);

unsigned int buckets_is_bucket_empty(Buckets *buckets, unsigned int bucket_number);

unsigned int buckets_is_buckets_empty(Buckets *buckets);

void print_bucket(Buckets buckets);

void buckets_free(Buckets *buckets);

#endif /* buckets_h */
