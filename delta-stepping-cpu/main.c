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

#define UINT_MAX 4294967295

#define HEAVY 1
#define LIGHT 0

typedef struct csr {
    unsigned int *data;
    unsigned int *indices;
    unsigned int *indptr;
    
    unsigned int num_nodes, num_arcs;
    
    unsigned int data_sz, indices_sz, indptr_sz;
} csr_graph;

typedef struct bkt {
    unsigned int num_buckets;
    unsigned int *mem_bucket;
    unsigned int delta;
} buckets;

void read_csr_graph(const char *graph_file_path, csr_graph *csr);
void find_weights(csr_graph csr, unsigned char *weights, unsigned int delta);
void set_inital_distances(csr_graph csr, unsigned int *dist);
unsigned int longest_short_path(csr_graph csr);
unsigned int find_pos_vertex_bucket(buckets buckets, unsigned int bucket, unsigned int vertex);
void remove_vertex_bucket(buckets *buckets, unsigned int bucket, unsigned int pos);
void add_vertex_bucket(buckets *buckets, unsigned int bucket, unsigned int vertex);
void relax(csr_graph csr, unsigned int vertex, unsigned int length, buckets *buckets, unsigned int *dist);
void initialize_buckets(buckets *buckets);

int is_buckets_empty(buckets buckets);
int is_specific_bucket_empty(buckets buckets, unsigned int bucket);

int main(int argc, const char * argv[]) {
    
    csr_graph csr;
    
    read_csr_graph("sample.data", &csr);
    
    // Delta
    unsigned int delta = 2;
    
    // Heavy or light edge
    unsigned char *weight = malloc(sizeof(unsigned char) * csr.data_sz);
    find_weights(csr, weight, delta);
    
    // Set initial distances
    unsigned int *dist = malloc(sizeof(unsigned int) * csr.num_nodes);
    set_inital_distances(csr, dist);
    
    // Buckets
    buckets buckts;
    buckts.num_buckets = (unsigned int) ceil((double)longest_short_path(csr) / (double)delta);
    buckts.mem_bucket = malloc(sizeof(unsigned int) * buckts.num_buckets * delta);
    buckts.delta = delta;
    initialize_buckets(&buckts);
    
    printf("%d\n", is_buckets_empty(buckts));
    
    // Relax s
    unsigned int s = 0;
    relax(csr, s, 0, &buckts, dist);
    
    // Deleted nodes bucket
    buckets deleted_bucket;
    deleted_bucket.num_buckets = buckts.num_buckets;
    
    free(csr.data);
    free(csr.indices);
    free(csr.indptr);
    
    free(weight);
    free(dist);
    
    free(buckts.mem_bucket);
    
    return 0;
}

void read_csr_graph(const char *graph_file_path, csr_graph *csr) {
    // Read graph file
    FILE *graph_file = fopen(graph_file_path, "r");
    
    // Read props
    unsigned int num_nodes, num_arcs, data_sz, indices_sz, indptr_sz;
    fscanf(graph_file, "%u", &num_nodes);
    fscanf(graph_file, "%u", &num_arcs);
    fscanf(graph_file, "%u", &data_sz);
    fscanf(graph_file, "%u", &indices_sz);
    fscanf(graph_file, "%u", &indptr_sz);
    
    // Allocate memory
    unsigned int *data_ptr = malloc(sizeof(unsigned int) * data_sz);
    unsigned int *col_idx_ptr = malloc(sizeof(unsigned int) * indices_sz);
    unsigned int *row_start_idx_ptr = malloc(sizeof(unsigned int) * indptr_sz);
    
    // Read data
    for (int i = 0; i < data_sz; i++) {
        fscanf(graph_file, "%u", &data_ptr[i]);
    }

    for (int i = 0; i < indices_sz; i++) {
        fscanf(graph_file, "%u", &col_idx_ptr[i]);
    }

    for (int i = 0; i < indptr_sz; i++) {
        fscanf(graph_file, "%u", &row_start_idx_ptr[i]);
    }

    // Output
    csr->data = data_ptr;
    csr->indices = col_idx_ptr;
    csr->indptr = row_start_idx_ptr;
    
    csr->data_sz = data_sz;
    csr->indices_sz = indices_sz;
    csr->indptr_sz = indptr_sz;
    
    csr->num_nodes = num_nodes;
    csr->num_arcs = num_arcs;
}

void find_weights(csr_graph csr, unsigned char *weights, unsigned int delta) {
    // Set heavy or light
    for (unsigned int i = 0; i < csr.data_sz; i++) {
        if (csr.data[i] > delta) {
            weights[i] = HEAVY;
        }
        else {
            weights[i] = LIGHT;
        }
    }
}

void set_inital_distances(csr_graph csr, unsigned int *dist) {
    for (unsigned int i = 0; i < csr.num_nodes; i++) {
        dist[i] = UINT_MAX;
    }
}

unsigned int longest_short_path(csr_graph csr) {
    unsigned int max = csr.data[0];
    for (unsigned int i = 1; i < csr.data_sz; i++) {
        if (csr.data[i] > max) {
            max = csr.data[i];
        }
    }
    return max;
}

void relax(csr_graph csr, unsigned int vertex, unsigned int length, buckets *buckets, unsigned int *dist) {
    if (length < dist[vertex]) {
        unsigned int cur_bucket = (unsigned int) floor((double)dist[vertex] / (double)buckets->delta);
        unsigned int tgt_bucket = (unsigned int) floor((double)length / (double)buckets->delta);
        
        if (cur_bucket < buckets->num_buckets) {
            unsigned int pos = find_pos_vertex_bucket(*buckets, cur_bucket, vertex);
            if (pos != UINT_MAX) {
                remove_vertex_bucket(buckets, cur_bucket, pos);
            }
        }
        
        add_vertex_bucket(buckets, tgt_bucket, vertex);
        dist[vertex] = length;
    }
}

unsigned int find_pos_vertex_bucket(buckets buckets, unsigned int bucket, unsigned int vertex) {
    // Return i, if found, UINT_MAX otherwise
    // Pos return the position
    for (unsigned int i = (bucket * buckets.delta); i < ((bucket + 1) * buckets.delta); i++) {
        if (buckets.mem_bucket[i] == vertex) {
            return i;
        }
    }
    return UINT_MAX;
}

void initialize_buckets(buckets *buckets) {
    for (int i = 0; i < buckets->num_buckets * buckets->delta; i++) {
        buckets->mem_bucket[i] = UINT_MAX;
    }
}

void remove_vertex_bucket(buckets *buckets, unsigned int bucket, unsigned int pos) {
    buckets->mem_bucket[pos] = UINT_MAX;
}

void add_vertex_bucket(buckets *buckets, unsigned int bucket, unsigned int vertex) {
    for (unsigned int i = (bucket * buckets->delta); i < ((bucket + 1) * buckets->delta); i++) {
        if (buckets->mem_bucket[i] == UINT_MAX) {
            buckets->mem_bucket[i] = vertex;
            break;
        }
    }
}

int is_buckets_empty(buckets buckets) {
    for (unsigned int i = 0; i < buckets.num_buckets * buckets.delta; i++) {
        if (buckets.mem_bucket[i] != UINT_MAX) {
            return 0;
        }
    }
    return 1;
}

int is_specific_bucket_empty(buckets buckets, unsigned int bucket) {
    for (unsigned int i = (bucket * buckets.delta); i < (bucket + 1) * buckets.delta; i++) {
        if (buckets.mem_bucket[i] != UINT_MAX) {
            return 0;
        }
    }
    return 1;
}