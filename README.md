<!-- omit in toc -->
# Lab 6

In this lab, you will implement three OS scheduling algorithms: first-come-first-serve (FCFS), round robin (RR), and our custom priority queue (min binary heap) scheduling algorithm. All three will use our `ArrayList` from Lab 3. This is an optimal data structure (compared to, e.g., a linked list) for the latter two algorithms but not the first, which would ideally use a queue (implemented by linked list).

You'll also become familiar with the `task_struct` that is used by the OS to manage a program running in memory. Here, we'll use only a subset of fields found in the [real Linux `task_struct`](https://github.com/torvalds/linux/blob/master/include/linux/sched.h#L778), namely the process id (pid) and priority, which we will manually assign values to.

<details open>
    <summary>Contents</summary>

- [Background reading](#background-reading)
    - [Repo structure](#repo-structure)
- [Input/output](#inputoutput)
- [Understand starter code](#understand-starter-code)
- [FCFS and round robin](#fcfs-and-round-robin)
- [Priority queue algorithm](#priority-queue-algorithm)
- [Submit your assignment](#submit-your-assignment)

</details>

## Background reading

1. Lab 3: ArrayList
    - The files [array_list.c](array_list.c) and [array_list.h](array_list.h) are included in this repo and are fully complete.
    - They are slightly modified from our Lab 3 solution. For example, the `array_list` struct now stores a `task_struct**` instead of an `int*`. Also, `remove_at_index` no longer shrinks the array capacity when the size is less than half the capacity.
2. [Binary Heaps and Priority Queues slides, COMP 210, Prof. Stotts](https://docs.google.com/presentation/d/1Lu5EZnL0-QrUU7Ub6foQ7um2RaDy-XEM/edit?usp=sharing&ouid=106881438081901765754&rtpof=true&sd=true)
    - The heap insertion and deletion algorithms will be re-implemented in this lab.
    - If you download the slides as PPTX and view in slideshow mode, there are several examples with animations.
    - If you took COMP 210 with Prof. Ghani, you may view his slides instead. The content should be roughly the same.
    - `insert` algorithm on slides 28-31, 33
    - `delMin` algorithm on slides 34-35
    - For simplicity, we will use successive `enqueue`s instead of the efficient build heap algorithm (slides 45-47).
    - Also for simplicity, we will not leave index 0 of the array unused (i.e., put a `NULL` value at index 0), unlike the examples shown in the slides above. In this repo, the first element of the min heap must be at index 0 of the array. Thus, given a node at index $i$, its parent is at index $\lfloor\frac{i-1}{2}\rfloor$, its left child (if it exists) is at index $2i+1$, and its right child (if it exists) is at index $2i+2$.
    - The formulas above are given to you in helper functions in [min_heap.c](min_heap.c). So, the small change above will not affect your implementation of the required functions if you use the provided helper functions.
    - We expect that you recall these concepts from COMP 210, a prerequisite of this class, or a corresponding data structures course at a different university.
3. For more details about OS scheduling algorithms, see [OSTEP Chapter 7](https://pages.cs.wisc.edu/~remzi/OSTEP/cpu-sched.pdf).

### Repo structure

Here is a description of each file and what you are expected to do (if anything) in each:

```text
.
|-- array_list.c - Important, already complete, and you should already be familiar
|-- array_list.h
|-- data - Inputs and outputs for main executable
|   |-- fcfs_out0.txt
|   |-- fcfs_out1.txt
|   |-- in0.txt
|   |-- in1.txt
|   |-- prqueue_out0.txt
|   |-- ...
|-- main.cpp
|-- Makefile
|-- min_heap.c - Important, implement some functions here
|-- min_heap.h
|-- README.md
|-- schedule.c - Important, implement all functions here
|-- schedule.h
|-- task.c - Important, complete, but you must understand the functions here
|-- task.h - Important, contains task_struct definition
|-- tests.cpp
|-- tests.hpp
|-- utils.cpp - Complete, and you may ignore
`-- utils.hpp
```

As usual, you must understand all files marked "important".

## Input/output

Example inputs and outputs for testing are in [data/](data). To show you what the code does, let's walk through a few examples.

[in0.txt](data/in0.txt) and [in1.txt](data/in1.txt) are common inputs for all output files. We select the specific scheduling algorithm via a command-line argument. Thus, all output files were generated with commands similar to the following: `./main fcfs < data/in0.txt > data/fcfs_out0.txt`.

Similar to previous labs, the output files don't show inputted text because the commands that generated them redirected only `stdout`. Here is an example that is run interactively in the terminal in which you can see the inputted values interspersed with the output:

```text
learncli$ ./main fcfs
Number of tasks: 4
Enter task 0's pid, priority, cycles (space-separated): 12 50 100
Enter task 1's pid, priority, cycles (space-separated): 13 1 80
Enter task 2's pid, priority, cycles (space-separated): 14 40 60
Enter task 3's pid, priority, cycles (space-separated): 15 0 40

+-----+----------+--------+-------------------+
| pid | priority | cycles | computed priority |
+-----+----------+--------+-------------------+
| 12  | 50       | 100    | 0.495050          |
+-----+----------+--------+-------------------+
| 13  | 1        | 80     | 0.012346          |
+-----+----------+--------+-------------------+
| 14  | 40       | 60     | 0.655738          |
+-----+----------+--------+-------------------+
| 15  | 0        | 40     | 0.000000          |
+-----+----------+--------+-------------------+

Task 12 ran for 100 cycles.
Task 12 completed.
Task 13 ran for 80 cycles.
Task 13 completed.
Task 14 ran for 60 cycles.
Task 14 completed.
Task 15 ran for 40 cycles.
Task 15 completed.
```

The main executable is run in `fcfs` mode, and the input is from `in0.txt`. Hopefully, this example is very clear and simple.

"cycles" refers to the number of CPU cycles a task will run for, which we assume is known.

FCFS and round robin do not use the priority and computed priority fields. Only the priority queue algorithm uses these fields. Here's an example of the priority queue algorithm running on the same input:

```text
learncli$ ./main prqueue
Number of tasks: 4
Enter task 0's pid, priority, cycles (space-separated): 12 50 100
Enter task 1's pid, priority, cycles (space-separated): 13 1 80
Enter task 2's pid, priority, cycles (space-separated): 14 40 60
Enter task 3's pid, priority, cycles (space-separated): 15 0 40

+-----+----------+--------+-------------------+
| pid | priority | cycles | computed priority |
+-----+----------+--------+-------------------+
| 15  | 0        | 40     | 0.000000          |
+-----+----------+--------+-------------------+
| 13  | 1        | 80     | 0.012346          |
+-----+----------+--------+-------------------+
| 14  | 40       | 60     | 0.655738          |
+-----+----------+--------+-------------------+
| 12  | 50       | 100    | 0.495050          |
+-----+----------+--------+-------------------+

Task 15 ran for 5 cycles.
Task 15 ran for 5 cycles.
Task 15 ran for 5 cycles.
Task 15 ran for 5 cycles.
Task 15 ran for 5 cycles.
Task 15 ran for 5 cycles.
Task 15 ran for 5 cycles.
Task 15 ran for 5 cycles.
Task 15 completed.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 ran for 5 cycles.
Task 13 completed.
Task 12 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 12 ran for 5 cycles.
Task 14 ran for 5 cycles.
Task 14 completed.
Task 12 ran for 5 cycles.
Task 12 completed.
```

Here, note that the printed table does not list the pid's in the order they were inputted, unlike FCFS and round robin, because each task was enqueued into the min heap based on the "computed priority" value.

## Understand starter code

If you haven't already, review the files marked "important" in [Repo structure](#repo-structure).

## FCFS and round robin

In `schedule.c`, implement all functions except `priority_queue`. These should be straightforward if you read the docstrings in `schedule.h`. For `fcfs` and `round_robin`, each unit test runs the `main` function with the appropriate command-line argument and input from [data/](data/) and compares the output to the expected output. To test and debug, you can do the same thing (i.e., run the `main` executable as described [above](#inputoutput)).

## Priority queue algorithm

If you have not read the Heap slides in [Background reading](#background-reading), do so now. If you haven't read through the given helper functions in `min_heap.c`, also do that.

In `min_heap.c`, implement the following functions, preferably in this order:

1. `bubble_up`
2. `enqueue`
3. `bubble_down`
4. `dequeue`

To simplify your code, you should use the given helper functions in the appropriate places. `bubble_up` and `bubble_down` are intended to be helper functions for `enqueue` and `dequeue`, respectively.

When implementing these functions, whenever you have to compare priority values of two nodes, use `compare_priorities_index(array_list* min_heap, int i, int j)`. Never directly compare the `priority` values of two nodes because for this scheduling algorithm, we consider the true priority to be `priority / (remaining_cycles + 1)`, which is automatically computed if you use the `compare_priorities_index` function.

After you're done with 1-4 and the unit tests for `enqueue` and `dequeue` pass, switch back to `schedule.c` and implement `priority_queue`, which should be straightforward.

## Submit your assignment

See the [instructions for assignment submission](https://github.com/Comp211-FA24/lab-00?tab=readme-ov-file#submit-your-assignment).
