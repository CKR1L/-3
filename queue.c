#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* create_queue() {
    Queue* p = (Queue*)malloc(sizeof(Queue));
    p->BegQ = p->EndQ = NULL;
    p->size = 0;
    return p;
}

void add_elem_queue(Queue* p, int value) {
    Elem* new_elem = (Elem*)malloc(sizeof(Elem));
    new_elem->inf = value;
    new_elem->link = NULL;
    if (p->EndQ == NULL) {
        p->BegQ = p->EndQ = new_elem;
    } else {
        p->EndQ->link = new_elem;
        p->EndQ = new_elem;
    }
    p->size++;
}

int remove_elem_queue(Queue* p) {
    if (is_empty(p)) {
        printf("В очереди нет элементов!");
        return -1;
    }
    Elem* temp = p->BegQ;
    int value = temp->inf;
    p->BegQ = p->BegQ->link;
    if (p->BegQ == NULL) {
        p->EndQ = NULL;
    }
    free(temp);
    p->size--;
    return value;
}

int is_empty(Queue* p) {
    return p->BegQ == NULL;
}

void free_queue(Queue* p) {
    while (!is_empty(p)) {
        remove_elem_queue(p);
    }
    free(p);
}

void print_queue(Queue* p) {
    Elem* current = p->BegQ;
    printf("Очередь [размер=%d]: ", p->size);
    while (current != NULL) {
        printf("%d", current->inf);
        current = current->link;
        if (current != NULL) printf(" → ");
    }
    printf("\n");
}

Queue* array_to_queue(int arr[], int size) {
    Queue* p = create_queue();
    for (int i = 0; i < size; i++) {
        add_elem_queue(p, arr[i]);
    }
    return p;
}