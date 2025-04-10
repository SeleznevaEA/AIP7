#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "struct.h"

void save_sorted_data(const char* filename, Scooter* scooters, int count, const char* sort_type) {
    char sorted_filename[256];
    snprintf(sorted_filename, sizeof(sorted_filename), "sorted_%s_%s", sort_type, filename);
    
    FILE* file = fopen(sorted_filename, "wb");
    if (file) {
        fwrite(scooters, sizeof(Scooter), count, file);
        fclose(file);
        printf("Отсортированные данные сохранены в %s\n", sorted_filename);
    } else {
        perror("Ошибка сохранения файла");
    }
}

void print_table_limited(Scooter* scooters, int count) {
    int display_count = (count > 100) ? 10 : count;
    
    printf("\n| %-4s | %-6s | %-10s | %-10s | %-8s | %-10s |\n", 
           "ID", "Year", "Brand", "Model", "Price", "Status");
    printf("|------|--------|------------|------------|----------|------------|\n");
    
    for (int i = 0; i < display_count; i++) {
        printf("| %-4s | %-6s | %-10s | %-10s | %-8s | %-10s |\n",
               scooters[i].id,
               scooters[i].year,
               scooters[i].brand,
               scooters[i].model,
               scooters[i].price,
               scooters[i].status);
    }
    
    if (count > 100) {
        printf("| ...  | ...    | ...        | ...        | ...      | ...        |\n");
        printf("Показано 10 из %d записей\n", count);
    }
    printf("|------|--------|------------|------------|----------|------------|\n\n");
}

// Функция создания случайных самокатов и записи в файл
Scooter* create_and_save_scooters(const char* filename, int* count) {
    srand(time(NULL));
    
    // Проверка корректности количества
    if (*count <= 0) {
        printf("Некорректное количество самокатов: %d. Установлено значение по умолчанию 10.\n", *count);
        *count = 10;
    }
    
    Scooter* scooters = (Scooter *)malloc(*count * sizeof(Scooter));
    if (!scooters) {
        perror("Ошибка выделения памяти");
        return NULL;
    }

    const char* brands[] = {"Xiaomi", "Ninebot", "Kugoo", "Hiley", "Smart"};
    const char* models[] = {"Pro 2", "Max", "S4", "V1", "X5"};
    const char* statuses[] = {"ready", "rent", "fixing"};

    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Не удалось открыть файл");
        free(scooters);
        return NULL;
    }

    for (int i = 0; i < *count; i++) {
        Scooter scooter;
        
        // Генерация уникального ID (1-1000)
        snprintf(scooter.id, sizeof(scooter.id), "%d", rand() % 1000 + 1);
        
        // Год выпуска (2010-2023)
        snprintf(scooter.year, sizeof(scooter.year), "20%d", 10 + rand() % 14);
        
        // Случайный бренд и модель
        strncpy(scooter.brand, brands[rand() % 5], sizeof(scooter.brand));
        strncpy(scooter.model, models[rand() % 5], sizeof(scooter.model));
        
        // Цена (200.99 - 999.99)
        snprintf(scooter.price, sizeof(scooter.price), "%d.99", 200 + rand() % 800);
        
        // Случайный статус
        strncpy(scooter.status, statuses[rand() % 3], sizeof(scooter.status));

        fwrite(&scooter, sizeof(Scooter), 1, file);
        scooters[i] = scooter;
    }

    fclose(file);
    printf("Успешно создано %d самокатов в файле %s\n", *count, filename);
    return scooters;
}

// Функция сравнения для сортировки (исправленная версия)
int compare_scooters(const Scooter *a, const Scooter *b) {
    // Сначала по id (как числа)
    int id_cmp = atoi(a->id) - atoi(b->id);
    if (id_cmp != 0) return id_cmp;
    
    // Затем по year (как числа)
    int year_cmp = atoi(a->year) - atoi(b->year);
    if (year_cmp != 0) return year_cmp;
    
    // Затем по brand (как строки)
    int brand_cmp = strcmp(a->brand, b->brand);
    if (brand_cmp != 0) return brand_cmp;
    
    // Затем по model
    int model_cmp = strcmp(a->model, b->model);
    if (model_cmp != 0) return model_cmp;
    
    // Затем по price (как float)
    float price_a = atof(a->price);
    float price_b = atof(b->price);
    if (price_a != price_b) return (price_a > price_b) ? 1 : -1;
    
    const char* status_order[] = {"ready", "rent", "fixing"};
    int a_status_rank = -1, b_status_rank = -1;

    for (int i = 0; i < 3; i++) {
        if (strcmp(a->status, status_order[i]) == 0) a_status_rank = i;
        if (strcmp(b->status, status_order[i]) == 0) b_status_rank = i;
    }

return a_status_rank - b_status_rank;
}



// Чтение из файла
int read_scooters(const char *filename, Scooter **scooters) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    int count = file_size / sizeof(Scooter);
    * scooters = (Scooter *)malloc(file_size);
    
    if (fread(*scooters, sizeof(Scooter), count, file) != count) {
        fclose(file);
        free(*scooters);
        return -1;
    }
    
    fclose(file);
    return count;
}


// Сортировка выбором 
void selection_sort_up(Scooter *scooters, int count) {
    for (int i = 0; i < count - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < count; j++) {
            if (compare_scooters(&scooters[j], &scooters[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Scooter temp = scooters[i];
            scooters[i] = scooters[min_idx];
            scooters[min_idx] = temp;
        }
    }
}

void selection_sort_down(Scooter *scooters, int count) {
    for (int i = 0; i < count - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < count; j++) {
            if (compare_scooters(&scooters[j], &scooters[min_idx]) > 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Scooter temp = scooters[i];
            scooters[i] = scooters[min_idx];
            scooters[min_idx] = temp;
        }
    }
}
// Сортировка пузырьком
void bubble_sort_up(Scooter *scooters, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compare_scooters(&scooters[j], &scooters[j+1]) > 0) {
                Scooter temp = scooters[j];
                scooters[j] = scooters[j+1];
                scooters[j+1] = temp;
            }
        }
    }
}
void bubble_sort_down(Scooter *scooters, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compare_scooters(&scooters[j], &scooters[j+1]) < 0) {
                Scooter temp = scooters[j];
                scooters[j] = scooters[j+1];
                scooters[j+1] = temp;
            }
        }
    }
}
// Сортировка шейкером (шейкерная сортировка)
void shaker_sort_up(Scooter *scooters, int count) {
    int left = 0;
    int right = count - 1;
    int swapped = 1;
    
    while (left < right && swapped) {
        swapped = 0;
        
        // Проход слева направо
        for (int i = left; i < right; i++) {
            if (compare_scooters(&scooters[i], &scooters[i+1]) > 0) {
                Scooter temp = scooters[i];
                scooters[i] = scooters[i+1];
                scooters[i+1] = temp;
                swapped = 1;
            }
        }
        if (!swapped)
            break;

        swapped = 0;
        right--;
        
        // Проход справа налево
        for (int i = right; i > left; i--) {
            if (compare_scooters(&scooters[i], &scooters[i-1]) < 0) {
                Scooter temp = scooters[i];
                scooters[i] = scooters[i-1];
                scooters[i-1] = temp;
                swapped = 1;
            }
        }
        left++;
    }
}
    void shaker_sort_down(Scooter *scooters, int count) {
        int left = 0;
        int right = count - 1;
        int swapped = 1;
        
        while (left < right && swapped) {
            swapped = 0;
            
            // Проход слева направо
            for (int i = left; i < right; i++) {
                if (compare_scooters(&scooters[i], &scooters[i+1]) < 0) {
                    Scooter temp = scooters[i];
                    scooters[i] = scooters[i+1];
                    scooters[i+1] = temp;
                    swapped = 1;
                }
            }
            if (!swapped)
                break;
    
            swapped = 0;
            right--;
            
            // Проход справа налево
            for (int i = right; i > left; i--) {
                if (compare_scooters(&scooters[i], &scooters[i-1]) > 0) {
                    Scooter temp = scooters[i];
                    scooters[i] = scooters[i-1];
                    scooters[i-1] = temp;
                    swapped = 1;
                }
            }
            left++;
        }
}
