#include "task.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

double compute_priority(task_struct* task) {
    return (double)task->priority / (task->remaining_cycles + 1);
}

int compare_priorities(task_struct* i, task_struct* j) {
    double i_prio = compute_priority(i);
    double j_prio = compute_priority(j);

    return fabs(i_prio - j_prio) < FLOAT_EPSILON ? 0 : i_prio < j_prio ? -1 : 1;
}
