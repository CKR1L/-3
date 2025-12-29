#ifndef FILE_H
#define FILE_H

void write_array_to_file(int arr[], int size, const char* filename);
void save_last_queue(int arr[], int size);
int* load_last_queue(int* size);
int* read_array_from_file(const char* filename, int* size);  
#endif