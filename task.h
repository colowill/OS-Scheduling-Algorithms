#ifndef TASK_H
#define TASK_H

#define FLOAT_EPSILON 0.0001

typedef struct {
    int pid;       // process id
    int priority;  // lower value means higher priority
                   // do not use directly, see compute_priority for how the true
                   // priority value is computed
    int remaining_cycles;  // remaining CPU cycles needed to complete
                           // this task, and this should not be set to a
                           // negative number (if it would be negative, simply
                           // set to 0)
} task_struct;

/**
 * See formula below. Because lower priority value means higher priority,
 * this formula prioritizes low priority value and high remaining_cycles value
 *
 * @note adds 1 to denominator to prevent division by 0
 * @param task
 * @return task->priority / (task->remaining_cycles + 1)
 */
double compute_priority(task_struct* task);

/**
 * @note function body uses the [ternary
 * operator](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Conditional_operator),
 * which is a concise way of representing if-else
 * @param i
 * @param j
 * @return return fabs(i_prio - j_prio) < FLOAT_EPSILON ? 0 : i_prio < j_prio ?
 * -1 : 1
 */
int compare_priorities(task_struct* i, task_struct* j);

#endif  // TASK_H
