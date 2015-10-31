//
//  csr_matrix.c
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#include "csr_matrix.h"

void read_csr_graph(const char *graph_file_path, csr_matrix *csr) {
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
    unsigned int max = 0;
    for (int i = 0; i < data_sz; i++) {
        fscanf(graph_file, "%u", &data_ptr[i]);
        if (data_ptr[i] > max) {
            max = data_ptr[i];
        }
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
    
    csr->max_edge = max;
}

void set_inital_distances(csr_matrix csr, unsigned int *dist) {
    for (unsigned int i = 0; i < csr.num_nodes; i++) {
        dist[i] = UINT_MAX;
    }
}
