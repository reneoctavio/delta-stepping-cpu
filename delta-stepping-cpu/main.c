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

void print_dist(unsigned int *dist, unsigned int num_vertices);

int main(int argc, const char * argv[]) {
    
    // Read CSR matrix
    csr_matrix csr;
    read_csr_graph("USA-road-d.NY.data", &csr);
    
    // Delta
    unsigned int delta = 5000;
    
    // Set initial distances
    unsigned int *dist = malloc(sizeof(unsigned int) * csr.num_nodes);
    set_inital_distances(csr, dist);
    
    // Initialize buckets
    Buckets buckets;
    buckets_create(csr.num_nodes, csr.max_edge, delta, &buckets);
    
    // Set initial vertex
    unsigned int s = 0;
    relax(csr, s, 0, &buckets, dist);
    
    // Set buckets iterator
    unsigned int i = 0;
    
    // Create Queue
    Queue Q;
    queue_initialize(&Q);
    
    // Outer loop
    while (!buckets_is_buckets_empty(&buckets)) {
        queue_clear(&Q);
        while (!buckets_is_bucket_empty(&buckets, i)) {
            // For every vertex in Bucket[i]
            for (unsigned int j = 0; j < delta; j++) {
                // Get vertex
                unsigned int vertex = *(buckets.bucket_position[i] + j);
                //printf("Vertex: %010u\n", vertex);
                
                // Break if there is no more vertices
                if (vertex == UINT_MAX) {
                    //break;
                }
                else {
                    // Move this vertex to Queue Q
                    enqueue(&Q, vertex);
                    //queue_print(Q);
                    // Remove vertex from Bucket[i]
                    buckets_remove_vertex(&buckets, i, vertex);
                    //print_bucket(buckets);
                    // Get this vertex edges
                    for (unsigned int idx = csr.indptr[vertex]; idx < csr.indptr[vertex + 1]; idx++) {
                        unsigned int tgt_vertex = csr.indices[idx];
                        unsigned int edge_value = csr.data[idx];
                        // Relax light edges
                        if (get_edge_weight(edge_value, delta) == LIGHT) {
                            unsigned int distance = dist[vertex] + edge_value;
                            printf("relax(%u, %u)\n", tgt_vertex, distance);
                            relax(csr, tgt_vertex, distance, &buckets, dist);
                            //print_bucket(buckets);
                            //print_dist(dist, csr.num_nodes);
                        }
                    }
                }
            }
        }
        // Clear current buckets and give room to the next bucket
        buckets_assign_next_bucket(&buckets, i);
        
        while (Q.first != NULL) {
            unsigned int vertex;
            dequeue(&Q, &vertex);
            //queue_print(Q);
            
            for (unsigned int idx = csr.indptr[vertex]; idx < csr.indptr[vertex + 1]; idx++) {
                unsigned int tgt_vertex = csr.indices[idx];
                unsigned int edge_value = csr.data[idx];
                // Relax light edges
                if (get_edge_weight(edge_value, delta) == HEAVY) {
                    unsigned int distance = dist[vertex] + edge_value;
                    relax(csr, tgt_vertex, distance, &buckets, dist);
                    printf("relax(%u, %u)\n", tgt_vertex, distance);
                    //print_dist(dist, csr.num_nodes);
                    //print_bucket(buckets);
                }
            }
        }
        
        i += 1;
    }
    
    print_dist(dist, csr.num_nodes);
    
    free(dist);
    
    free(csr.data);
    free(csr.indices);
    free(csr.indptr);
    
    return 0;
}


void print_dist(unsigned int *dist, unsigned int num_vertices) {
    printf("Distance: ");
    for (unsigned int i = 0; i < num_vertices; i++) {
        printf("%u ", dist[i]);
    }
    printf("\n");
}
