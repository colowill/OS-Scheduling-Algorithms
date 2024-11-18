#include "array_list.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

array_list* create_array_list(int initial_capacity) {
    array_list* list = (array_list*)malloc(sizeof(array_list));
    if (list == NULL) return NULL;
    list->array = (task_struct**)malloc(sizeof(task_struct) * initial_capacity);
    if (list->array == NULL) return NULL;
    list->size = 0;
    list->capacity = initial_capacity;
    return list;
}

void ensure_capacity(array_list* list, int min_capacity) {
    if (list->capacity < min_capacity) {
        list->capacity = min_capacity;
        task_struct** ptr = (task_struct**)realloc(
            list->array, sizeof(task_struct*) * list->capacity);
        assert(ptr != NULL);
        list->array = ptr;
    }
}

void free_array_list(array_list** list) {
    if (*list == NULL) return;
    free((*list)->array);
    free((*list));
    *list = NULL;
}

bool contains(array_list* list, int pid) {
    for (int i = 0; i < list->size; i++)
        if (list->array[i]->pid == pid) return true;
    return false;
}

void add(array_list* list, task_struct* task) {
    if (list->size == list->capacity) ensure_capacity(list, list->capacity * 2);
    list->array[list->size++] = task;
}

task_struct* remove_at_index(array_list* list, int index) {
    validate_index(list, index);

    task_struct* rv = list->array[index];
    for (int i = index; i < list->size - 1; i++)
        list->array[i] = list->array[i + 1];
    list->size--;

    return rv;
}

task_struct* get(array_list* list, int index) {
    validate_index(list, index);
    return list->array[index];
}

void set(array_list* list, int index, task_struct* task) {
    validate_index(list, index);
    list->array[index] = task;
}

void clear(array_list* list) {
    task_struct** ptr = (task_struct**)realloc(
        list->array, sizeof(task_struct*) * DEFAULT_CAPACITY);
    assert(ptr != NULL);
    list->array = ptr;
    list->size = 0;
    list->capacity = DEFAULT_CAPACITY;
}

void validate_index(array_list* list, int index) {
    assert(index >= 0 && index < list->size);
}
