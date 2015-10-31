//
//  queue.c
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/30/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#include "queue.h"

void enqueue(Queue *queue, unsigned int data) {
    // Allocate new node
    QueueNode *temp = malloc(sizeof(QueueNode));
    
    // Set new node props
    temp->data = data;
    temp->next = NULL;
    
    // Check if it is first insertion
    if (queue->first == NULL && queue->last == NULL) {
        queue->first = queue->last = temp;
        return;
    }
    
    // If not, put it at end
    queue->last->next = temp;
    queue->last = temp;
}

void dequeue(Queue *queue, unsigned int *data) {
    QueueNode *temp = queue->first;
    
    // Check if queue is empty
    if (queue->first == NULL) {
        printf("Queue is Empty\n");
        return;
    }
    
    // If it is last, NULL everyone
    if (queue->first == queue->last) {
        queue->first = queue->last = NULL;
    }
    else {
        queue->first = queue->first->next;
    }
    *data = temp->data;
    free(temp);
}

unsigned int peek(Queue queue) {
    return queue.first->data;
}

void queue_clear(Queue *queue) {
    while(queue->first != NULL) {
        unsigned int dump;
        dequeue(queue, &dump);
    }
}

void queue_initialize(Queue *queue) {
    queue->first = NULL;
    queue->last = NULL;
}

void queue_print(Queue queue) {
    QueueNode *temp = queue.first;
    printf("Queue: ");
    while (temp != NULL) {
        printf("%02u ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}
