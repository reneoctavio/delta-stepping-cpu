//
//  main.c
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/28/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <libiomp/omp.h>

#include "buckets.h"
#include "csr_matrix.h"
#include "helper.h"
#include "queue.h"

#define MSG_RELAX 0
#define MSG_DISTC 0
#define MSG_BUCKT 0
#define MSG_QUEUE 0

void print_dist(unsigned int *dist, unsigned int num_vertices);

int main(int argc, const char * argv[]) {
    
    // Read CSR matrix
    csr_matrix csr;
    read_csr_graph("USA-road-d.NY.data", &csr);
    
    // Delta
    unsigned int delta = 50;
    
    // Set initial distances
    unsigned int *dist = malloc(sizeof(unsigned int) * csr.num_nodes);
    set_inital_distances(csr, dist);
    
    // Initialize buckets
    Buckets buckets;
    buckets_create(csr.num_nodes, csr.max_edge, delta, &buckets);
    
    // Set initial vertex
    unsigned int s = 0;
    relax(csr, s, 0, &buckets, dist);
    
#if MSG_BUCKT
    print_bucket(buckets);
#endif
    
    // Set buckets iterator
    unsigned int i = 0;
    
    // Create Queue
    Queue Q;
    queue_initialize(&Q);
    
    // Outer loop
    while (!buckets_is_buckets_empty(&buckets)) {
        queue_clear(&Q);
            
        // Get bucket position
        unsigned int bucket_pos = (i % buckets.num_buckets);
        SingleNode **bucket = &buckets.list[bucket_pos];
        
        // Get vertex
        unsigned int vertex = pop(bucket);
        
        while (vertex != UINT_MAX) {
            // Add to Q
            enqueue(&Q, vertex);
#if MSG_QUEUE
            queue_print(Q);
#endif
#if MSG_BUCKT
            print_bucket(buckets);
#endif
            // Get this vertex edges
            for (unsigned int idx = csr.indptr[vertex]; idx < csr.indptr[vertex + 1]; idx++) {
                unsigned int tgt_vertex = csr.indices[idx];
                unsigned int edge_value = csr.data[idx];
                // Relax light edges
                if (get_edge_weight(edge_value, delta) == LIGHT) {
                    unsigned int distance = dist[vertex] + edge_value;
#if MSG_RELAX
                    printf("Light Relax(%u, %u)\n", tgt_vertex, distance);
#endif
                    relax(csr, tgt_vertex, distance, &buckets, dist);
#if MSG_BUCKT
                    print_bucket(buckets);
#endif
                }
            }
            // Get next vertex if available
            vertex = pop(bucket);
        }
        
        i++;
        buckets.cur_bucket_num = i;
        
        while (Q.first != NULL) {
            unsigned int vertex;
            dequeue(&Q, &vertex);
#if MSG_QUEUE
            queue_print(Q);
#endif
            
            for (unsigned int idx = csr.indptr[vertex]; idx < csr.indptr[vertex + 1]; idx++) {
                unsigned int tgt_vertex = csr.indices[idx];
                unsigned int edge_value = csr.data[idx];
                // Relax light edges
                if (get_edge_weight(edge_value, delta) == HEAVY) {
                    unsigned int distance = dist[vertex] + edge_value;
#if MSG_RELAX
                    printf("Heavy Relax(%u, %u)\n", tgt_vertex, distance);
#endif
                    relax(csr, tgt_vertex, distance, &buckets, dist);
#if MSG_DISTC
                    print_dist(dist, csr.num_nodes);
#endif
#if MSG_BUCKT
                    print_bucket(buckets);
#endif
                }
            }
        }
    }
    
    print_dist(dist, csr.num_nodes);
    
    free(dist);
    free(csr.data);
    free(csr.indices);
    free(csr.indptr);
    buckets_free(&buckets);
    
    return 0;
}


void print_dist(unsigned int *dist, unsigned int num_vertices) {
    printf("Distance: ");
    for (unsigned int i = 0; i < num_vertices; i++) {
        if (dist[i] == UINT_MAX) {
            printf(" -- ");
        }
        else {
            printf(" %02u ", dist[i]);
        }
    }
    printf("\n");
}
