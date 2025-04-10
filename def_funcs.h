#ifndef FUNCTIONS
#define FUNCTIONS


void save_sorted_data(const char* filename, Scooter* scooters, int count, const char* sort_type);

void print_table_limited(Scooter* scooters, int count) ;

Scooter* create_and_save_scooters(const char* filename, int* count);

int compare_scooters(const Scooter *a, const Scooter *b);

int read_scooters(const char *filename, Scooter **scooters);

void selection_sort_up(Scooter *scooters, int count);

void selection_sort_down(Scooter *scooters, int count);

void bubble_sort_up(Scooter *scooters, int count);
void bubble_sort_down(Scooter *scooters, int count);

void shaker_sort_up(Scooter *scooters, int count);
void shaker_sort_down(Scooter *scooters, int count);

#endif