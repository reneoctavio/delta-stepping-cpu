//
//  queue.h
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#ifndef queue_h
#define queue_h

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode {
    unsigned int data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode *first;
    QueueNode *last;
} Queue;

void enqueue(Queue *queue, unsigned int data);

void dequeue(Queue *queue, unsigned int *data);

unsigned int peek(Queue queue);

void queue_clear(Queue *queue);

void queue_initialize(Queue *queue);

void queue_print(Queue queue);

#endif /* queue_h */
