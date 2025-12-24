#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"
#include "queue.h"

void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        int tmp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = tmp;        
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    int tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return (i + 1);
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

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