#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
int* queue_to_array(Queue* p, int* size) {
    *size = p->size;
    int* arr = (int*)malloc(*size * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка при выделении памяти!");
        return NULL;
    }
    Elem* current = p->BegQ;
    for (int i = 0; i < *size; i++) {
        arr[i] = current->inf;
        current = current->link;
    }
    return arr;
}
Elem* find_min_prev(Queue* p, Elem* start) {
    if (start == NULL || start->link == NULL) return NULL;
    
    Elem* min_prev = start;
    Elem* current = start->link;
    
    while (current->link != NULL) {
        if (current->link->inf < min_prev->link->inf) {
            min_prev = current;
        }
        current = current->link;
    }
    
    return min_prev;
}

void queue_selection_sort(Queue* p) {
    if (p->size <= 1) return;
    Elem dummy;
    dummy.link = p->BegQ;
    p->BegQ = &dummy;

    Elem *sorted_tail = &dummy;

    while (sorted_tail->link != NULL) {
        Elem *min_prev = find_min_prev(p, sorted_tail);
        Elem *min_node;
        
        if (min_prev == NULL) {
            min_node = sorted_tail->link;
            sorted_tail = sorted_tail->link;
        } else {
            min_node = min_prev->link;
            min_prev->link = min_node->link;
            
            min_node->link = sorted_tail->link;
            sorted_tail->link = min_node;
            sorted_tail = min_node;
        }
    }

    p->BegQ = dummy.link;   
    Elem *current = p->BegQ;
    while (current->link != NULL) {
        current = current->link;
    }
    p->EndQ = current;
}

void queue_quick_sort(Queue* p) {
    if (p->size <= 1) return;
    int pivot = p->BegQ->inf;
    Queue less = {NULL, NULL, 0};
    Queue equal = {NULL, NULL, 0};
    Queue greater = {NULL, NULL, 0};
    while (p->size > 0) {
        int value = dequeue(p);
        if (value < pivot) {
            enqueue(&less, value);
        } else if (value == pivot) {
            enqueue(&equal, value);
        } else {
            enqueue(&greater, value);
        }
    }    
    queue_quick_sort(&less);
    queue_quick_sort(&greater);

    while (less.size > 0) {
        enqueue(p, dequeue(&less));
    }
    while (equal.size > 0) {
        enqueue(p, dequeue(&equal));
    }
    while (greater.size > 0) {
        enqueue(p, dequeue(&greater));
    }
}
