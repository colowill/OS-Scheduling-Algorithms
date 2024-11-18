// Minimum binary heap of task_struct* backed by array_list
// First element at index 0 (i.e., do not use a NULL value at index 0)
// To implement, see README Background reading section with COMP 210 slides
// enqueue and dequeue are the only tested functions, and bubble_up and
// bubble_down are intended to be helper functions for enqueue and dequeue

#include "min_heap.h"

#include <stdlib.h>

#include "task.h"

void enqueue(array_list* min_heap, task_struct* task) {
    if (min_heap->size == min_heap->capacity)
        ensure_capacity(min_heap, min_heap->size+1);
    add(min_heap, task);
    bubble_up(min_heap, min_heap->size-1);
}

task_struct* dequeue(array_list* min_heap) {
    
    if (min_heap->size < 1) return NULL;
    task_struct* temp = get_min(min_heap);
    swap(min_heap, 0, min_heap->size - 1);
    remove_at_index(min_heap, min_heap->size - 1);
    bubble_down(min_heap, 0);
    return temp;
}

task_struct* get_min(array_list* min_heap) { return get(min_heap, 0); }

void bubble_up(array_list* min_heap, int index) {
    while (index > 0) {
        int parent_index = get_parent(index);
        if (compare_priorities_index(min_heap, index, parent_index) == -1){
            swap(min_heap, index, parent_index);
            index = parent_index;
        } else break;
    }
}

void bubble_down(array_list* min_heap, int index) {
    while (has_left(min_heap, index)) {
        int left = get_left(index);
        int right = get_right(index);
        int small = left;  

        if (has_right(min_heap, index) &&
            compare_priorities_index(min_heap, right, left) == -1) {
            small = right;
        }

        if (compare_priorities_index(min_heap, index, small) == -1) {
            break;
        }

        swap(min_heap, index, small);
        index = small;
    }
}

void swap(array_list* min_heap, int i, int j) {
    task_struct* temp = get(min_heap, i);
    set(min_heap, i, get(min_heap, j));
    set(min_heap, j, temp);
}

int compare_priorities_index(array_list* min_heap, int i, int j) {
    task_struct* task_i = get(min_heap, i);
    task_struct* task_j = get(min_heap, j);
    return compare_priorities(task_i, task_j);
}

bool is_leaf(array_list* min_heap, int index) {
    return !has_left(min_heap, index);
}

int get_parent(int index) { return (index - 1) / 2; }

int get_left(int index) { return 2 * index + 1; }

int get_right(int index) { return 2 * index + 2; }

bool has_left(array_list* min_heap, int index) {
    return get_left(index) < min_heap->size;
}

bool has_right(array_list* min_heap, int index) {
    return get_right(index) < min_heap->size;
} 
