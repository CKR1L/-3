#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "queue.h"
#include "sorting.h"
#include "file.h"
#include "benchmark.h"

int* input_array_dynamic(int* size) {
    printf("Введите элементы массива через пробел (для завершения нажмите Enter):\n");
    printf("> ");
    
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return NULL;
    }
    
    line[strcspn(line, "\n")] = 0;
    
    if (strlen(line) == 0) {
        printf("Не введено ни одного числа!\n");
        return NULL;
    }
    
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isdigit(line[i]) && 
            line[i] != ' ' && 
            line[i] != '\t' && 
            line[i] != '+' && 
            line[i] != '-' &&
            line[i] != '\0') {
            
            if ((line[i] == '+' || line[i] == '-') && 
                i + 1 < strlen(line) && 
                isdigit(line[i + 1])) {
                continue;
            }
            
            printf("Ошибка: обнаружен недопустимый символ в \n");
            printf("Вводите только целые числа и пробелы!\n");
            return NULL;
        }
    }
    
    int count = 0;
    char* ptr = line;
    while (*ptr) {
        while (*ptr && isspace(*ptr)) ptr++;
        if (*ptr && !isspace(*ptr)) {
            if (*ptr == '+' || *ptr == '-' || isdigit(*ptr)) {
                count++;
            }
            while (*ptr && !isspace(*ptr)) ptr++;
        }
    }
    
    if (count == 0) {
        printf("Не найдено чисел во вводе!\n");
        return NULL;
    }
    
    int* arr = (int*)malloc(count * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    
    ptr = line;
    int index = 0;
    
    while (*ptr && index < count) {
        while (*ptr && isspace(*ptr)) ptr++;
        
        if (*ptr) {
            int value;
            int chars_read;
            
            if (sscanf(ptr, "%d%n", &value, &chars_read) == 1) {
                char temp[20];
                sprintf(temp, "%d", value);
                int expected_chars = strlen(temp);
                
                if (ptr[0] == '+' || ptr[0] == '-') {
                    expected_chars++;
                }
                
                if (chars_read < expected_chars) {
                    printf("Ошибка: обнаружен недопустимый символ в числе\n", 
                           (ptr - line) + chars_read + 1);
                    free(arr);
                    return NULL;
                }
                
                arr[index] = value;
                index++;
                ptr += chars_read;
            } else {
                printf("Ошибка: невозможно прочитать число");
                free(arr);
                return NULL;
            }
        }
    }
    
    *size = count;
    printf(" Успешно считано %d чисел\n", count);
    return arr;
}

void print_array(int arr[], int size) {
    if (size <= 20) {
        printf("[");
        for (int i = 0; i < size; i++) {
            printf("%d", arr[i]);
            if (i < size - 1) printf(", ");
        }
        printf("]\n");
    } else {
        printf("[");
        for (int i = 0; i < 5; i++) {
            printf("%d, ", arr[i]);
        }
        printf("..., ");
        for (int i = size - 5; i < size; i++) {
            printf("%d", arr[i]);
            if (i < size - 1) printf(", ");
        }
        printf("]\n");
    }
}

void print_time(double time_ms, const char* sort_name) {
    if (time_ms < 1.0) {
        printf("%s: %.5f мс (%.0f мкс)\n", sort_name, time_ms, time_ms * 1000);
    } else if (time_ms < 1000.0) {
        printf("%s: %.5f мс\n", sort_name, time_ms);
    } else {
        printf("%s: %.5f мс (%.3f с)\n", sort_name, time_ms, time_ms / 1000.0);
    }
}

void show_menu() {
    printf("\n=======================================================\n");
    printf("                МЕНЮ ПРОГРАММЫ СОРТИРОВКИ                \n");
    printf("=========================================================\n");
    printf("1. Ввести массив вручную и отсортировать\n");
    printf("2. Запустить тестирование на тестовых файлах\n");
    printf("3. Выйти из программы\n");
    printf("\nВыберите действие (1-3): ");
}

void show_graph() {
    printf("\n Запуск построения графика...\n");
    
    FILE* results_file = fopen("results.txt", "r");
    if (results_file == NULL) {
        printf("  Файл results.txt не найден.\n");
        printf("  Сначала запустите тестирование для создания файла с результатами.\n");
        return;
    }
    fclose(results_file);
    
    FILE* graph_script = fopen("graph.py", "r");
    if (graph_script == NULL) {
        printf("   Файл graph.py не найден в текущей директории.\n");
        printf("   Убедитесь, что graph.py находится в той же папке, что и эта программа.\n");
        return;
    }
    fclose(graph_script);
    
    printf(" Запускаю Python-скрипт для построения графика...\n");
    printf("  (Откроется новое окно с графиком)\n\n");
    
    int result = system("python3 graph.py");
    
    if (result == 0) {
        printf("\n График успешно построен и сохранен как graph.png\n");
        printf("  Результаты также сохранены в файле results.txt\n");
    } else {
        printf("\n Не удалось построить график.\n");
        printf("  Убедитесь, что установлен Python и библиотека matplotlib.\n");
        printf("  Для установки matplotlib выполните: pip install matplotlib\n");
    }
}

int main() {
    int choice;
    
    printf("==========================================================\n");
    printf("                ПРОГРАММА СОРТИРОВКИ МАССИВОВ             \n");
    printf("==========================================================\n");
    
    while (1) {
        show_menu();
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Неверный ввод. Введите число от 1 до 3.\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice == 1) {
            printf("\n========================================================\n");
            printf("               РЕЖИМ 1: РУЧНОЙ ВВОД МАССИВА              \n");
            printf("=========================================================\n");
            
            int size;
            int* arr = input_array_dynamic(&size);
            
            if (arr == NULL) {
                printf("Пожалуйста, попробуйте снова.\n");
                continue;
            }
            
            printf("\nИсходный массив: ");
            print_array(arr, size);
            
            Queue* p = array_to_queue(arr, size);
            printf("Очередь из массива: ");
            print_queue(p);
            printf("\n");
            
            int* arr_selection = (int*)malloc(size * sizeof(int));
            int* arr_quick = (int*)malloc(size * sizeof(int));
            
            if (arr_selection == NULL || arr_quick == NULL) {
                printf("Ошибка выделения памяти!\n");
                free(arr);
                if (arr_selection) free(arr_selection);
                if (arr_quick) free(arr_quick);
                free_queue(p);
                continue;
            }
            
            memcpy(arr_selection, arr, size * sizeof(int));
            memcpy(arr_quick, arr, size * sizeof(int));
            
            clock_t start = clock();
            selection_sort(arr_selection, size);
            clock_t end = clock();
            double selection_time = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            
            start = clock();
            quick_sort(arr_quick, 0, size - 1);
            end = clock();
            double quick_time = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            
            printf("==========================================\n");
            printf("   РЕЗУЛЬТАТЫ СОРТИРОВКИ:\n");
            printf("==========================================\n");
            
            printf("\n   ВРЕМЯ ВЫПОЛНЕНИЯ:\n");
            printf("------------------------------------\n");
            print_time(selection_time, "Сортировка выбором");
            print_time(quick_time, "Быстрая сортировка");
            
            if (selection_time > 0 && quick_time > 0) {
                double speedup = selection_time / quick_time;
                printf("----------------------------------\n");
                printf("  Быстрая сортировка быстрее в %.2f раз\n", speedup);
            }
            
            printf("\n  ОТСОРТИРОВАННЫЕ МАССИВЫ:\n");
            printf("--------------------------\n");
            printf("Сортировка выбором:   ");
            print_array(arr_selection, size);
            
            printf("Быстрая сортировка:   ");
            print_array(arr_quick, size);
            
            int correct = 1;
            for (int i = 0; i < size - 1; i++) {
                if (arr_selection[i] > arr_selection[i + 1]) {
                    correct = 0;
                    break;
                }
            }
            
            printf("\n ");
            if (correct) {
                printf("Массивы отсортированы корректно (проверено)");
            } else {
                printf("Обнаружена ошибка в сортировке!");
            }
            printf("\n");
            
            write_array_to_file(arr, size, "orig.txt");
            write_array_to_file(arr_selection, size, "sorted_selection.txt");
            write_array_to_file(arr_quick, size, "sorted_quick.txt");
            
            printf("\n  ФАЙЛЫ СОХРАНЕНЫ:\n");
            printf("---------------------------------------\n");
            printf("  - Исходный массив:           orig.txt\n");
            printf("  - Сортировка выбором:        sorted_selection.txt\n");
            printf("  - Быстрая сортировка:        sorted_quick.txt\n");
            
            free(arr);
            free(arr_selection);
            free(arr_quick);
            free_queue(p);
            
            printf("\nНажмите Enter для возврата в меню...");
            getchar();
            
        } else if (choice == 2) {
            printf("\n========================================================\n");
            printf("          РЕЖИМ 2: ТЕСТИРОВАНИЕ НА ТЕСТОВЫХ ФАЙЛАХ        \n");
            printf("==========================================================\n");
            
            
            benchmark_sorting();
            
            printf("\n==========================================================\n");
            printf("          ПОКАЗАТЬ ГРАФИК РЕЗУЛЬТАТОВ?                   \n");
            printf("==========================================================\n");
            printf("Хотите увидеть график сравнения производительности?\n");
            printf("1. Да, показать график\n");
            printf("2. Нет, вернуться в меню\n");
            printf("\nВыберите действие (1-2): ");
            
            int graph_choice;
            if (scanf("%d", &graph_choice) != 1) {
                while (getchar() != '\n');
                printf("Неверный ввод.\n");
            } else {
                while (getchar() != '\n');
                
                if (graph_choice == 1) {
                    show_graph();
                }
            }
            
            printf("\nНажмите Enter для возврата в меню...");
            getchar();
            
        } else if (choice == 3) {
            printf("\n------------------------------------------------------\n");
            printf("               ВЫХОД ИЗ ПРОГРАММЫ                       \n");
            printf("--------------------------------------------------------\n");
            printf("Спасибо за использование программы! До свидания!\n");
            break;
            
        } else {
            printf("Неверный выбор. Пожалуйста, введите число от 1 до 3.\n");
        }
    }
    
    return 0;
}