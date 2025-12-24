#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file.h"

void write_array_to_file(int arr[], int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка при открытии файла: %s\n", filename);
        return;
    }
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d", arr[i]);
        if (i < size - 1) {
            fprintf(file, " ");
        }
    }
    fclose(file);
}

int* read_array_from_file(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Ошибка открытия файла: %s\n", filename);
        *size = 0;
        return NULL;
    }
    int capacity = 100;
    int* arr = (int*)malloc(capacity * sizeof(int));
    *size = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        if (*size >= capacity) {
            capacity *= 2;
            arr = (int*)realloc(arr, capacity * sizeof(int));
        }
        arr[(*size)++] = num;
    }
    fclose(file);
    return arr;
}