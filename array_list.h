// Dynamic array

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdbool.h>

#include "task.h"

/**
 * Dynamic array of task_struct*
 *
 * Each array_list has a capacity. The capacity is the size of the array
 * used to store the elements in the list. The capacity is always at least as
 * large as the list size. As elements are added to an array_list, its capacity
 * grows automatically. The growth policy uses a simple doubling or halving
 * approach.
 */
typedef struct {
    task_struct** array;  // backing array
    int size;             // number of elements currently in the list
    int capacity;  // maximum number of elements (i.e., length of the array)
} array_list;

// Default capacity of cleared array_list
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static int DEFAULT_CAPACITY = 10;
#pragma GCC diagnostic pop

/**
 * Creates array_list with capacity initial_capacity
 *
 * @param initial_capacity
 * @return array_list*, NULL if malloc fails
 */
array_list* create_array_list(int initial_capacity);

/**
 * Increases the capacity of this array_list instance, if necessary, to ensure
 * that it can hold at least the number of elements specified by the minimum
 * capacity argument
 *
 * @param list
 * @param min_capacity desired minimum capacity
 */
void ensure_capacity(array_list* list, int min_capacity);

/**
 * Frees memory allocated for this array_list and mutates the argument to be
 * NULL
 *
 * @param list pointer to list whose memory is to be freed
 */
void free_array_list(array_list** list);

/**
 * Returns true if this list contains a task_struct with the given pid
 *
 * @param list
 * @param pid pid whose presence in this list is to be tested
 * @return true if this list contains a task_struct with the given pid
 */
bool contains(array_list* list, int pid);

/**
 * Appends the specified element to the end of this list
 *
 * @note Before the element insertion, if list->size == list->capacity, calls
 * ensure_capacity to double capacity
 * @param list
 * @param task element to be appended to this list
 */
void add(array_list* list, task_struct* task);

/**
 * Removes the element at the specified position in this list. Shifts any
 * subsequent elements to the left (subtracts one from their indices)
 *
 * @note Does not call realloc when list is under half capacity
 * @param list
 * @param index the index of the element to be removed
 * @return removed element
 */
task_struct* remove_at_index(array_list* list, int index);

/**
 * Returns the element at the specified position in this list
 *
 * @param list
 * @param index index of the element to return
 * @return element at specified index
 */
task_struct* get(array_list* list, int index);

/**
 * Replaces the element at the specified position in this list with the
 * specified element
 *
 * @param list
 * @param index index of the element to replace
 * @param task element to be stored at the specified position
 */
void set(array_list* list, int index, task_struct* task);

/**
 * Removes all of the elements from this list. The list will be empty after this
 * call returns
 *
 * @note list->array must be realloc'ed and have the new capacity
 * DEFAULT_CAPACITY
 * @param list
 */
void clear(array_list* list);

/**
 * Validates index with assert statement
 *
 * @note If index 0 contains NULL (i.e., for min heap), then this functions
 * slightly differently
 * @param list
 * @param index
 */
void validate_index(array_list* list, int index);

#endif  // ARRAY_LIST_H
