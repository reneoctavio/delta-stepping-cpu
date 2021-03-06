//
//  helper.c
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#include "helper.h"

unsigned int get_edge_weight(unsigned int edge, unsigned int delta) {
    if (edge > delta) {
        return HEAVY;
    }
    return LIGHT;
}

void relax(csr_matrix csr, unsigned int vertex, unsigned int distance, Buckets *buckets, unsigned int *dist) {
    if (distance < dist[vertex]) {
        
        unsigned int cur_bucket, tgt_bucket;
        
        if (dist[vertex] % buckets->delta == 0) {
            cur_bucket = dist[vertex] / buckets->delta;
        }
        else {
            cur_bucket = (unsigned int) floor((double)dist[vertex] / (double)buckets->delta);
        }
        
        if (distance % buckets->delta == 0) {
            tgt_bucket = distance / buckets->delta;
        }
        else {
            tgt_bucket = (unsigned int) floor((double)distance / (double)buckets->delta);
        }
        
        // Remove from a Bucket no. floor(dist[v]/delta) if possible
        if ((buckets->cur_bucket_num + buckets->num_buckets) > cur_bucket) {
            buckets_remove_vertex(buckets, cur_bucket, vertex);
        }
        
        // Add at Bucket no. floor(distance/delta)
        buckets_add_vertex(buckets, tgt_bucket, vertex);
        
        // Update vertex distance
        dist[vertex] = distance;
    }
}