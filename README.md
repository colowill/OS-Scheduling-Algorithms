<!-- omit in toc -->
## OS Scheduling Algorithms [FCFS, RR, PRQ]

In this lab, you will implement three OS scheduling algorithms: first-come-first-serve (FCFS), round robin (RR), and our custom priority queue (min binary heap) scheduling algorithm. All three will use our `ArrayList` from Lab 3. This is an optimal data structure (compared to, e.g., a linked list) for the latter two algorithms but not the first, which would ideally use a queue (implemented by linked list).

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

And so on...

Task 12 ran for 5 cycles.
Task 12 completed.
```
