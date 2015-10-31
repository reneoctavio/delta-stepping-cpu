//
//  lists.h
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/31/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#ifndef lists_h
#define lists_h

#include "constants.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct SingleNode {
    unsigned int value;
    struct SingleNode *next;
} SingleNode;

void print_list(SingleNode * head);

void push(SingleNode ** head, unsigned int value);

unsigned int pop(SingleNode ** head);

unsigned int remove_last(SingleNode * head);

unsigned int remove_by_index(SingleNode ** head, unsigned int idx);

void remove_by_value(SingleNode ** head, unsigned int value);

#endif /* lists_h */
