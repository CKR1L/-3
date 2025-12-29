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

void save_last_queue(int arr[], int size) {
    FILE* file = fopen("last_queue.txt", "w");  
    if (file == NULL) {
        return;  
    }
    
    fprintf(file, "%d\n", size);  
    
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", arr[i]);
    }
    
    fclose(file);
}

int* load_last_queue(int* size) {
    FILE* file = fopen("last_queue.txt", "r");
    if (file == NULL) {
        return NULL;  
    }
    
    // Читаем размер
    if (fscanf(file, "%d", size) != 1) {
        fclose(file);
        return NULL;
    }
    
    if (*size <= 0 || *size > 1000000) {  
        fclose(file);
        return NULL;
    }
    
    int* arr = (int*)malloc(*size * sizeof(int));
    if (arr == NULL) {
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < *size; i++) {
        if (fscanf(file, "%d", &arr[i]) != 1) {
            free(arr);
            fclose(file);
            return NULL;
        }
    }
    
    fclose(file);
    return arr;
}