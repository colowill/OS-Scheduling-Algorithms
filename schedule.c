// See docstrings in schedule.h

#include "schedule.h"

#include <stdio.h>
#include <stdlib.h>

#include "min_heap.h"

void run_to_completion(task_struct* task) {
    printf("Task %d ran for %d cycles.",task->pid,task->remaining_cycles);
    task->remaining_cycles=0;
    printf("\nTask %d completed.\n",task->pid);
}

void run_with_quantum(task_struct* task, int quantum) {
    if (task->remaining_cycles <= quantum) run_to_completion(task);
    else {
        task->remaining_cycles-=quantum;
        printf("Task %d ran for %d cycles.\n",task->pid,quantum);
    }
}

void fcfs(array_list* queue) {
    while (queue->size!=0) {
        task_struct* temp = get(queue, 0);
        run_to_completion(temp);
        remove_at_index(queue, 0);
    } 
}

void round_robin(array_list* list, int quantum) {
    int index = -1;
    while (list->size!=0) {
        index = (index+1) % list->size;
        task_struct* task = get(list,index);
        run_with_quantum(task, quantum);
        if (task->remaining_cycles==0) {
            remove_at_index(list, index);
            index--;
        }
    }
}

void priority_queue(array_list* min_heap, int quantum) {
    while (min_heap->size != 0) {
        task_struct* task = get(min_heap, 0);
        dequeue(min_heap);
        run_with_quantum(task, quantum);
        if (task->remaining_cycles != 0) {
            enqueue(min_heap, task);
        }
    }
}
