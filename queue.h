#ifndef QUEUE_H
#define QUEUE_H
typedef struct Elem {
    int inf;
    struct Elem* link;
}Elem;

typedef struct Queue{
    Elem* BegQ;
    Elem* EndQ;
    int size;
} Queue;
Queue* create_queue();
void add_elem_queue(Queue* p, int value);
int remove_elem_queue(Queue *p);
int is_empty(Queue *p);
void free_queue(Queue *p);
void print_queue(Queue *p);
void enqueue(Queue *p, int value);
int dequeue(Queue *p);
Queue* array_to_queue(int arr[], int size);
int* queue_to_array(Queue* p, int* size);

#endif