#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <math.h>

#include "array_list.h"
#include "task.h"

// For preemptive scheduling algorithms, the maximum time slice that a task can
// be run for before a context switch to another task
#define QUANTUM 5

/**
 * Runs task on the CPU for its total number of remaining cycles
 *
 * Print "Task {pid} ran for {remaining_cycles} cycle(s)."
 * Set task's remaining_cycles to 0.
 * Print "Task {pid} completed."
 *
 * @param task
 * @return void
 */
void run_to_completion(task_struct* task);

/**
 * Runs task on the CPU for a maximum time quantum
 *
 * If remaining_cycles <= quantum, call run_to_completion
 *
 * Else, decrement task's remaining_cycles by quantum
 * and print "Task {pid} ran for {quantum} cycle(s)."
 *
 * @param task
 * @param quantum
 * @return void
 */
void run_with_quantum(task_struct* task, int quantum);

/**
 * First-come-first-serve scheduling algorithm
 *
 * While the queue is not empty:
 *  - Get the task at the front of the queue
 *  - Run the task to completion
 *  - Remove the task from the queue
 *
 * @param queue
 * @return void
 */
void fcfs(array_list* queue);

/**
 * Round robin scheduling algorithm
 *
 * Initialize int index to -1
 *
 * While the list is not empty:
 *  - Set index to (index + 1) % list->size
 *  - Run the task at that index for the given quantum, mutating its
 * remaining_cycles
 *  - If the task is complete, remove it from the list and decrement index
 *
 * @param list
 * @param quantum
 * @return void
 */
void round_robin(array_list* list, int quantum);

/**
 * Priority queue (implemented by min heap) scheduling algorithm
 *
 * While the min_heap is not empty:
 *  - Dequeue the task with minimum computed priority
 *  - Run the task for the given quantum, mutating its remaining_cycles
 *  - If the task is not complete, enqueue it back into the min_heap
 *
 * @param list
 * @param quantum
 * @return void
 */
void priority_queue(array_list* min_heap, int quantum);

#endif  // SCHEDULE_H
