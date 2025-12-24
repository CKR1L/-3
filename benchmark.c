#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"
#include "file.h"

void benchmark_sorting() {
    printf("===========================================================\n");
    printf("          ТЕСТИРОВАНИЕ ПРОИЗВОДИТЕЛЬНОСТИ СОРТИРОВОК      \n");
    printf("===========================================================\n");
    
    int sizes[] = {100, 500, 1000, 5000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("┌──────────────┬──────────────────────┬──────────────────────┬──────────┐\n");
    printf("│   Размер     │  Сорт. выбором (с)   │ Быстрая сорт. (с)    │ Ускорение│\n");
    printf("├──────────────┼──────────────────────┼──────────────────────┼──────────┤\n");
    
    FILE* results_file = fopen("results.txt", "w");
    if (results_file != NULL) {
        fprintf(results_file, "Size Selection Quick\n");
    }
    
    for (int i = 0; i < num_sizes; i++) {
        char filename[50];
        sprintf(filename, "test_data%d.txt", sizes[i]);
        
        int size;
        int* arr = read_array_from_file(filename, &size);
        
        if (arr == NULL) {
            printf("│ %-12s │   ФАЙЛ НЕ НАЙДЕН                         │\n", filename);
            if (results_file != NULL) {
                fprintf(results_file, "%d 0 0\n", sizes[i]);
            }
            continue;
        }
        
        printf("│ Тестирование %-4d │ ", sizes[i]);
        fflush(stdout);
        
        int* arr_selection = (int*)malloc(size * sizeof(int));
        int* arr_quick = (int*)malloc(size * sizeof(int));
        
        if (arr_selection == NULL || arr_quick == NULL) {
            printf("ОШИБКА ПАМЯТИ                   │\n");
            free(arr);
            if (results_file != NULL) {
                fprintf(results_file, "%d 0 0\n", sizes[i]);
            }
            continue;
        }
        
        for (int j = 0; j < size; j++) {
            arr_selection[j] = arr[j];
            arr_quick[j] = arr[j];
        }
        
        clock_t start = clock();
        selection_sort(arr_selection, size);
        clock_t end = clock();
        double selection_time = ((double)(end - start)) / CLOCKS_PER_SEC; 
        
        start = clock();
        quick_sort(arr_quick, 0, size - 1);
        end = clock();
        double quick_time = ((double)(end - start)) / CLOCKS_PER_SEC; 
        
        double speedup = (quick_time > 0) ? selection_time / quick_time : 0;
        
        printf("%10.6f │ %10.6f │  x%5.3f │\n", 
               selection_time, quick_time, speedup);
        
        
        if (results_file != NULL) {
            fprintf(results_file, "%d %.6f %.6f\n", sizes[i], selection_time, quick_time);
        }
        
        free(arr);
        free(arr_selection);
        free(arr_quick);
    }
    
    printf("└──────────────┴──────────────────────┴──────────────────────┴──────────┘\n");
    
    if (results_file != NULL) {
        fclose(results_file);
        printf("\n  Результаты сохранены в файл results.txt\n");
        printf("   Для построения графика будет использован этот файл.\n");
    }
    
    printf("\n=========================================================\n");
    printf("                       ВЫВОДЫ:                             \n");
    printf("===========================================================\n");
    
    printf("1. Быстрая сортировка значительно быстрее при больших объемах данных\n");
    printf("2. Сортировка выбором эффективна только для небольших массивов\n");
    printf("3. Разница в производительности растет с увеличением размера данных\n");
    printf("\nТестирование завершено успешно!\n");
}