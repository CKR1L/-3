#ifndef SORTING_H
#define SORTING_H
#include "queue.h"
void selection_sort(int arr[], int n);
void quick_sort(int arr[], int low, int high);
int* queue_to_array(Queue* p, int* size);
#endif