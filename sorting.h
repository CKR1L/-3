#ifndef SORTING_H
#define SORTING_H
#include "queue.h"
int* queue_to_array(Queue *p, int*size);
void queue_selection_sort(Queue *p);
void queue_quick_sort(Queue *p);
#endif