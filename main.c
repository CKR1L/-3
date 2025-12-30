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
            line[i] != '-') {
            
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
                    printf("Ошибка: обнаружен недопустимый символ в числе\n");
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
    printf("        МЕНЮ ПРОГРАММЫ СОРТИРОВКИ ОЧЕРЕДЕЙ              \n");
    printf("=========================================================\n");
    printf("1. Ввести массив вручную и отсортировать как очередь\n");
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
    
    int result = system("python3 graph.py");
    
    if (result == 0) {
        printf("\n График успешно построен и сохранен как graph.png\n");
    } else {
        printf("\n Не удалось построить график.\n");
        printf("  Убедитесь, что установлен Python и библиотека matplotlib.\n");
        printf("  Для установки matplotlib выполните: pip install matplotlib\n");
    }
}
void show_last_results(){
    printf("ПРЕДЫДУЩАЯ ОЧЕРЕДЬ\n");
    int orig_size;
    int* orig_arr = read_array_from_file("orig.txt", &orig_size);
    if (orig_arr != NULL && orig_size > 0) {
        printf("Размер: %d элементов\n", orig_size);
        printf("Массив: ");
        print_array(orig_arr, orig_size);
        free(orig_arr);
    } else {
        printf("\nФайл sorted_queue_selection.txt не найден.\n");
    }
    int sel_size;
    int* sel_arr = read_array_from_file("sorted_queue_selection.txt", &sel_size);
    
    if (sel_arr != NULL && sel_size > 0) {
        printf("\n--- ОТСОРТИРОВАНО ВЫБОРОМ (из sorted_queue_selection.txt) ---\n");
        printf("Размер: %d элементов\n", sel_size);
        printf("Массив: ");
        print_array(sel_arr, sel_size);
        free(sel_arr);
    } else {
        printf("\nФайл sorted_queue_selection.txt не найден.\n");
    }
}
int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], "--file") == 0) {
        show_last_results();
        return 0;
    }
    int choice;
    
    printf("==========================================================\n");
    printf("        ПРОГРАММА СОРТИРОВКИ ОЧЕРЕДЕЙ                      \n");
    printf("==========================================================\n");
    
    int last_size = 0;
    int* last_arr = load_last_queue(&last_size);
    
    if (last_arr != NULL && last_size > 0) {
        printf("\n--- ПОСЛЕДНЯЯ ОТСОРТИРОВАННАЯ ОЧЕРЕДЬ ---\n");
        printf("Размер: %d элементов\n", last_size);
        printf("Очередь: ");
        
        Queue* last_queue = array_to_queue(last_arr, last_size);
        print_queue(last_queue);
        
        free_queue(last_queue);
        free(last_arr);
        printf("-------------------------------------------\n\n");
    } else {
        printf("\n(Предыдущих результатов нет)\n\n");
    }
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
            printf("     РЕЖИМ 1: РУЧНОЙ ВВОД И СОРТИРОВКА ОЧЕРЕДИ          \n");
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
            
            Queue* queue_selection = array_to_queue(arr, size);
            Queue* queue_quick = array_to_queue(arr, size);
            
            clock_t start = clock();
            queue_selection_sort(queue_selection);
            clock_t end = clock();
            double selection_time = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            
            start = clock();
            queue_quick_sort(queue_quick);
            end = clock();
            double quick_time = ((double)(end - start)) * 1000 / CLOCKS_PER_SEC;
            
            printf("==========================================\n");
            printf("   РЕЗУЛЬТАТЫ СОРТИРОВКИ ОЧЕРЕДЕЙ:\n");
            printf("==========================================\n");
            
            printf("\n   ВРЕМЯ ВЫПОЛНЕНИЯ:\n");
            printf("------------------------------------\n");
            print_time(selection_time, "Сортировка выбором (очередь)");
            print_time(quick_time, "Быстрая сортировка (очередь)");
            
            if (selection_time > 0 && quick_time > 0) {
                double speedup = selection_time / quick_time;
                printf("----------------------------------\n");
                printf("  Быстрая сортировка быстрее в %.2f раз\n", speedup);
            }
            
            int size1, size2;
            int* arr_selection = queue_to_array(queue_selection, &size1);
            int* arr_quick = queue_to_array(queue_quick, &size2);
            
            printf("\n  ОТСОРТИРОВАННЫЕ ДАННЫЕ:\n");
            printf("--------------------------\n");
            printf("Сортировка выбором (очередь):   ");
            print_array(arr_selection, size1);
            
            printf("Быстрая сортировка (очередь):   ");
            print_array(arr_quick, size2);
            
            int correct_selection = 1;
            int correct_quick = 1;
            
            for (int i = 0; i < size1 - 1; i++) {
                if (arr_selection[i] > arr_selection[i + 1]) {
                    correct_selection = 0;
                    break;
                }
            }
            
            for (int i = 0; i < size2 - 1; i++) {
                if (arr_quick[i] > arr_quick[i + 1]) {
                    correct_quick = 0;
                    break;
                }
            }
            
            printf("\n ");
            if (correct_selection && correct_quick) {
                printf("Обе очереди отсортированы корректно");
            } else {
                printf("Обнаружена ошибка в сортировке!");
                if (!correct_selection) printf(" (выбором)");
                if (!correct_quick) printf(" (быстрая)");
            }
            printf("\n");
            
            save_last_queue(arr_quick, size2);
            printf("  Отсортированная очередь сохранена в last_queue.txt\n");   
            write_array_to_file(arr, size, "orig.txt");
            write_array_to_file(arr_selection, size1, "sorted_queue_selection.txt");
            write_array_to_file(arr_quick, size2, "sorted_queue_quick.txt");
            
            printf("\n  ФАЙЛЫ СОХРАНЕНЫ:\n");
            printf("---------------------------------------\n");
            printf("  - Исходный массив:           orig.txt\n");
            printf("  - Очередь (сорт. выбором):   sorted_queue_selection.txt\n");
            printf("  - Очередь (быстрая сорт.):   sorted_queue_quick.txt\n");
            
            free(arr);
            free(arr_selection);
            free(arr_quick);
            free_queue(p);
            free_queue(queue_selection);
            free_queue(queue_quick);
            
            printf("\nНажмите Enter для возврата в меню...");
            getchar();
            
        } else if (choice == 2) {
            printf("\n========================================================\n");
            printf("      РЕЖИМ 2: ТЕСТИРОВАНИЕ НА ТЕСТОВЫХ ФАЙЛАХ          \n");
            printf("==========================================================\n");
            
            benchmark_sorting();
            
            printf("\nХотите увидеть график сравнения производительности?\n");
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