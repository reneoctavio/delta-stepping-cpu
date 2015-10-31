//
//  helper.h
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#ifndef helper_h
#define helper_h

#include "constants.h"
#include "buckets.h"
#include "csr_matrix.h"

#include <stdio.h>

unsigned int get_edge_weight(unsigned int edge, unsigned int delta);

void relax(csr_matrix csr, unsigned int vertex, unsigned int distance, Buckets *buckets, unsigned int *dist);

#endif /* helper_h */
