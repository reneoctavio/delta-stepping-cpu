//
//  lists.c
//  delta-stepping-cpu
//
//  Created by Rene Octavio Queiroz Dias on 10/31/15.
//  Copyright © 2015 Rene Octavio Queiroz Dias. All rights reserved.
//

#include "lists.h"

void print_list(SingleNode * head) {
    SingleNode * current = head;
    
    while (current != NULL) {
        printf("%d\n", current->value);
        current = current->next;
    }
}

void push(SingleNode ** head, unsigned int value) {
    SingleNode * new_node;
    new_node = malloc(sizeof(SingleNode));
    
    new_node->value = value;
    new_node->next = *head;
    *head = new_node;
}

unsigned int pop(SingleNode ** head) {
    unsigned int retvalue = UINT_MAX;
    SingleNode * next_node = NULL;
    
    if (*head == NULL) {
        return UINT_MAX;
    }
    
    next_node = (*head)->next;
    retvalue = (*head)->value;
    free(*head);
    *head = next_node;
    
    return retvalue;
}

unsigned int remove_last(SingleNode * head) {
    int retvalue = UINT_MAX;
    /* if there is only one item in the list, remove it */
    if (head->next == NULL) {
        retvalue = head->value;
        free(head);
        head = NULL;
        return retvalue;
    }
    
    SingleNode * current = head;
    
    while (current->next->next != NULL) {
        current = current->next;
    }
    retvalue = current->value;
    current = NULL;
    free(current);
    return retvalue;
}

unsigned int remove_by_index(SingleNode ** head, unsigned int idx) {
    unsigned int retvalue = UINT_MAX;
    SingleNode * current = *head;
    SingleNode * temp_node = NULL;
    
    if (idx == 0) {
        return pop(head);
    }
    
    for (unsigned int i = 0; i < (idx - 1); i++) {
        if (current->next == NULL) {
            return UINT_MAX;
        }
        current = current->next;
    }
    
    temp_node = current->next;
    retvalue = temp_node->value;
    current->next = temp_node->next;
    free(temp_node);
    
    return retvalue;
    
}

void remove_by_value(SingleNode ** head, unsigned int value) {
    SingleNode * current = *head;
    
    unsigned int idx = 0;
    while (current != NULL) {
        if (current->value == value) {
            remove_by_index(head, idx);
            break;
        }
        idx++;
        current = current->next;
    }
}