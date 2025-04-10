#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>



#include "struct.h"
#include "def_funcs.h"



int main() {
    const char *filename = "scooters.bin";
    int count;
    char choice;
    
    // Запрос количества самокатов
    printf("Введите количество самокатов для создания: ");
    while (scanf("%d", &count) != 1 || count <= 0) {
        printf("Некорректный ввод. Введите положительное число: ");
        while (getchar() != '\n');
    }
    
    // Создаем начальные данные
    Scooter *initial_scooters = create_and_save_scooters(filename, &count);
    if (!initial_scooters) return 1;
    
    printf("\nСоздано %d самокатов и сохранено в %s\n", count, filename);
    print_table_limited(initial_scooters, count);
    free(initial_scooters);
    
    // Основной цикл программы
    while (1) {
        printf("\nМеню:\n");
        printf("1 - Показать исходные данные\n");
        printf("2 - Сортировка выбором возрастание\n");
        printf("3 - Сортировка выбором убывание\n");
        printf("4 - Сортировка пузырьком возрастание\n");
        printf("5 - Сортировка пузырьком убывание\n");
        printf("6 - Шейкерная сортировка возрастание\n");
        printf("7 - Шейкерная сортировка убывание\n");
        printf("8 - Создать новые исходные данные\n");
        printf("0 - Выход\n");
        printf("Ваш выбор: ");
        scanf(" %c", &choice);
        
        if (choice == '0') break;
        
        Scooter *scooters;
        count = read_scooters(filename, &scooters);
        if (count <= 0) {
            printf("Ошибка чтения файла\n");
            continue;
        }
        
        switch(choice) {
            case '1':
                printf("\nИсходные данные (%d самокатов):", count);
                print_table_limited(scooters, count);
                break;
                
            case '2': {
                Scooter *copy = (Scooter *)malloc(count * sizeof(Scooter));
                if (!copy) {
                    printf("Ошибка выделения памяти\n");
                    break;
                }
                memcpy(copy, scooters, count * sizeof(Scooter));
                
                clock_t start = clock();
                selection_sort_up(copy, count);
                clock_t end = clock();
                
                printf("\nОтсортировано методом выбора возрастание (время: %.6f сек)\n", 
                      (double)(end - start) / CLOCKS_PER_SEC);
                print_table_limited(copy, count);
                save_sorted_data(filename, copy, count, "selection");
                free(copy);
                break;
            }

            case '3': {
                Scooter *copy = (Scooter *)malloc(count * sizeof(Scooter));
                if (!copy) {
                    printf("Ошибка выделения памяти\n");
                    break;
                }
                memcpy(copy, scooters, count * sizeof(Scooter));
                
                clock_t start = clock();
                selection_sort_down(copy, count);
                clock_t end = clock();
                
                printf("\nОтсортировано методом выбора убывание (время: %.6f сек)\n", 
                      (double)(end - start) / CLOCKS_PER_SEC);
                print_table_limited(copy, count);
                save_sorted_data(filename, copy, count, "selection");
                free(copy);
                break;
            }
                
            case '4': {
                Scooter *copy = (Scooter *)malloc(count * sizeof(Scooter));
                if (!copy) {
                    printf("Ошибка выделения памяти\n");
                    break;
                }
                memcpy(copy, scooters, count * sizeof(Scooter));
                
                clock_t start = clock();
                bubble_sort_up(copy, count);
                clock_t end = clock();
                
                printf("\nОтсортировано методом пузырька возрастание(время: %.6f сек)\n", 
                      (double)(end - start) / CLOCKS_PER_SEC);
                print_table_limited(copy, count);
                save_sorted_data(filename, copy, count, "bubble");
                free(copy);
                break;
            }
            case '5': {
                Scooter *copy = (Scooter *)malloc(count * sizeof(Scooter));
                if (!copy) {
                    printf("Ошибка выделения памяти\n");
                    break;
                }
                memcpy(copy, scooters, count * sizeof(Scooter));
                
                clock_t start = clock();
                bubble_sort_down(copy, count);
                clock_t end = clock();
                
                printf("\nОтсортировано методом пузырька убывание(время: %.6f сек)\n", 
                      (double)(end - start) / CLOCKS_PER_SEC);
                print_table_limited(copy, count);
                save_sorted_data(filename, copy, count, "bubble");
                free(copy);
                break;
            }   
            case '6': {
                Scooter *copy = (Scooter *)malloc(count * sizeof(Scooter));
                if (!copy) {
                    printf("Ошибка выделения памяти\n");
                    break;
                }
                memcpy(copy, scooters, count * sizeof(Scooter));
                
                clock_t start = clock();
                shaker_sort_up(copy, count);
                clock_t end = clock();
                
                printf("\nОтсортировано шейкерной сортировкой возрастание (время: %.6f сек)\n", 
                      (double)(end - start) / CLOCKS_PER_SEC);
                print_table_limited(copy, count);
                save_sorted_data(filename, copy, count, "shaker");
                free(copy);
                break;
            }
            case '7': {
                Scooter *copy = (Scooter *)malloc(count * sizeof(Scooter));
                if (!copy) {
                    printf("Ошибка выделения памяти\n");
                    break;
                }
                memcpy(copy, scooters, count * sizeof(Scooter));
                
                clock_t start = clock();
                shaker_sort_down(copy, count);
                clock_t end = clock();
                
                printf("\nОтсортировано шейкерной сортировкой убывание(время: %.6f сек)\n", 
                      (double)(end - start) / CLOCKS_PER_SEC);
                print_table_limited(copy, count);
                save_sorted_data(filename, copy, count, "shaker");
                free(copy);
                break;
            }  
            case '8': {
                free(scooters);
                
                printf("\nВведите новое количество самокатов: ");
                while (scanf("%d", &count) != 1 || count <= 0) {
                    printf("Некорректный ввод. Введите положительное число: ");
                    while (getchar() != '\n');
                }
                
                initial_scooters = create_and_save_scooters(filename, &count);
                if (!initial_scooters) return 1;
                
                printf("\nСоздано %d новых самокатов и сохранено в %s\n", count, filename);
                print_table_limited(initial_scooters, count);
                free(initial_scooters);
                continue;
            }
                
            default:
                printf("Неверный выбор\n");
        }
        
        free(scooters);
    }
    
    printf("Программа завершена.\n");
    return 0;
}