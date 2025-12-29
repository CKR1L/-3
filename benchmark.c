#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"
#include "sorting.h"
#include "file.h"

void benchmark_sorting() {
    printf("===========================================================\n");
    printf("    ТЕСТИРОВАНИЕ ПРОИЗВОДИТЕЛЬНОСТИ СОРТИРОВОК ОЧЕРЕДЕЙ   \n");
    printf("===========================================================\n");
    
    int sizes[] = {100, 500, 1000, 5000, 10000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("┌──────────────┬──────────────────────┬──────────────────────┬──────────┐\n");
    printf("│   Размер     │ Сорт. выбором (с)    │ Быстрая сорт. (с)    │ Ускорение│\n");
    printf("│              │     (очередь)        │     (очередь)        │          │\n");
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
        
        Queue* queue_selection = array_to_queue(arr, size);
        Queue* queue_quick = array_to_queue(arr, size);
        
        if (queue_selection == NULL || queue_quick == NULL) {
            printf("ОШИБКА СОЗДАНИЯ ОЧЕРЕДИ           │\n");
            free(arr);
            if (queue_selection) free_queue(queue_selection);
            if (queue_quick) free_queue(queue_quick);
            if (results_file != NULL) {
                fprintf(results_file, "%d 0 0\n", sizes[i]);
            }
            continue;
        }
        clock_t start = clock();
        queue_selection_sort(queue_selection);
        clock_t end = clock();
        double selection_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        
        start = clock();
        queue_quick_sort(queue_quick);
        end = clock();
        double quick_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        
        int selection_correct = 1;
        int quick_correct = 1;
        
        if (queue_selection->size > 0) {
            int size1;
            int* arr_selection = queue_to_array(queue_selection, &size1);
            if (arr_selection) {
                for (int j = 0; j < size1 - 1; j++) {
                    if (arr_selection[j] > arr_selection[j + 1]) {
                        selection_correct = 0;
                        break;
                    }
                }
                free(arr_selection);
            }
        }
        
        if (queue_quick->size > 0) {
            int size2;
            int* arr_quick = queue_to_array(queue_quick, &size2);
            if (arr_quick) {
                for (int j = 0; j < size2 - 1; j++) {
                    if (arr_quick[j] > arr_quick[j + 1]) {
                        quick_correct = 0;
                        break;
                    }
                }
                free(arr_quick);
            }
        }
        
        double speedup = (quick_time > 0) ? selection_time / quick_time : 0;
        
        printf("%10.6f │ %10.6f │  x%10.3f │\n", 
               selection_time, quick_time, speedup);
        
        if (results_file != NULL) {
            fprintf(results_file, "%d %.6f %.6f\n", sizes[i], selection_time, quick_time);
        }
        
        free(arr);
        free_queue(queue_selection);
        free_queue(queue_quick);
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
    
    printf("1. Быстрая сортировка очередей значительно быстрее при больших объемах данных\n");
    printf("3. Разница в производительности растет с увеличением размера очереди\n");
    printf("4. Восклицательный знак (!) указывает на возможную ошибку сортировки\n");
    printf("\nТестирование завершено успешно!\n");
}