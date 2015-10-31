//
//  csr_matrix.h
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#ifndef csr_matrix_h
#define csr_matrix_h

#include "constants.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct csr_matrix {
    unsigned int *data;
    unsigned int *indices;
    unsigned int *indptr;
    
    unsigned int num_nodes, num_arcs;
    
    unsigned int data_sz, indices_sz, indptr_sz;
    
    unsigned int max_edge;
} csr_matrix;

void read_csr_graph(const char *graph_file_path, csr_matrix *csr);

void set_inital_distances(csr_matrix csr, unsigned int *dist);

unsigned int max_edge(csr_matrix csr);

#endif /* csr_matrix_h */
