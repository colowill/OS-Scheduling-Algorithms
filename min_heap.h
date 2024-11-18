// Minimum binary heap of task_struct* backed by array_list
// First element at index 0 (i.e., do not use a NULL value at index 0)

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include "array_list.h"
#include "stdbool.h"

/**
 * Inserts task into min_heap using the standard insertion algorithm
 *
 * @param min_heap
 * @param task
 */
void enqueue(array_list* min_heap, task_struct* task);
/**
 * Removes the task at index 0 and returns it using the standard delete_min
 * algorithm
 *
 * @param min_heap
 * @return task_struct*
 */
task_struct* dequeue(array_list* min_heap);

/**
 * Returns the task at index 0
 *
 * @param min_heap
 * @return task_struct*
 */
task_struct* get_min(array_list* min_heap);

/**
 * Standard bubble-up algorithm that bubbles up the element at the given index
 *
 * @note used in enqueue
 * @note must use compare_priorities_index, don't use priority value directly
 * @param min_heap
 * @param index
 */
void bubble_up(array_list* min_heap, int index);

/**
 * Standard bubble-down algorithm that bubbles down the element at the given
 * index
 *
 * @note used in dequeue
 * @note must use compare_priorities_index, don't use priority value directly
 * @param min_heap
 * @param index
 */
void bubble_down(array_list* min_heap, int index);

/**
 * Swaps the min_heap elements at indices i and j
 *
 * @note used in bubble_up and bubble_down
 * @param min_heap
 * @param i
 * @param j
 */
void swap(array_list* min_heap, int i, int j);

/**
 * Compares the computed priorities of the min_heap elements at indices i and j
 *
 * @param min_heap
 * @param a
 * @param b
 * @return -1 if i_prio < j_prio | 0 if i_prio ~ j_prio | 1 if i_prio > j_prio
 */
int compare_priorities_index(array_list* min_heap, int i, int j);

bool is_leaf(array_list* min_heap, int index);

int get_parent(int index);

int get_left(int index);

int get_right(int index);

bool has_left(array_list* min_heap, int index);

bool has_right(array_list* min_heap, int index);

#endif  // MIN_HEAP_H
