#include "utils.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "min_heap.h"
#include "schedule.h"

static std::unordered_map<std::string, scheduling_algo> const scheduling_algos =
    {{"fcfs", FCFS}, {"rr", RR}, {"prqueue", PRQUEUE}};

void print_usage_and_exit() {
    std::cerr << "Usage: ./main fcfs|rr|prqueue\n\n";
    std::cerr << "\tfcfs|rr|prqueue is the scheduling algorithm to use\n";
    std::cerr << "\tRun interactively with the above command or "
                 "non-interactively by redirecting stdin from text file, e.g. "
                 "./main fcfs < data/in0.txt\n";
    exit(1);
}

scheduling_algo parse_cli(int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect argc: " << argc << "\n\n";
        print_usage_and_exit();
    }

    std::string argv1 = argv[1];
    auto it = scheduling_algos.find(argv1);
    if (it == scheduling_algos.end()) {
        std::cerr << argv1 << " is not a valid scheduling algorithm\n\n";
        print_usage_and_exit();
        // Unreachable, the above function exits, this is to suppress compiler
        // error
        exit(1);
    } else
        return it->second;
}

void print_table_as_grid(std::vector<std::vector<std::string>> table) {
    // Width of each column is the length of longest element in each column
    std::vector<std::size_t> column_widths;
    for (std::size_t col = 0; col < table[0].size(); col++) {
        std::size_t max_width = 0;
        for (const auto& row : table)
            max_width = std::max(max_width, row[col].size());
        column_widths.push_back(max_width);
    }

    // Helper function that prints a horizontal line of chars
    auto print_line = [&](const char left, const char middle, const char right,
                          const char fill) {
        std::cout << left;
        for (size_t col = 0; col < column_widths.size(); ++col) {
            std::cout << std::string(column_widths[col] + 2,
                                     fill);  // Add padding
            if (col < column_widths.size() - 1) std::cout << middle;
        }
        std::cout << right << "\n";
    };

    // Print top border
    print_line('+', '+', '+', '-');

    // Print each row
    for (long unsigned int row = 0; row < table.size(); row++) {
        // Print row content
        for (size_t col = 0; col < table[row].size(); col++)
            std::cout << "| " << std::left << std::setw(column_widths[col])
                      << table[row][col] << " ";
        std::cout << "|\n";

        if (row == table.size() - 1)
            print_line('+', '+', '+', '-');
        else
            print_line('+', '+', '+', '-');
    }
}

std::vector<std::vector<std::string>> array_list_to_table(array_list* list) {
    std::vector<std::vector<std::string>> rv;
    std::vector<std::string> header{"pid", "priority", "cycles",
                                    "computed priority"};
    rv.push_back(header);

    for (int i = 0; i < list->size; i++) {
        task_struct* task = get(list, i);
        std::vector<std::string> row{std::to_string(task->pid),
                                     std::to_string(task->priority),
                                     std::to_string(task->remaining_cycles),
                                     std::to_string(compute_priority(task))};
        rv.push_back(row);
    }

    return rv;
}

void sim(scheduling_algo scheduling_algo) {
    int num_tasks;
    std::cout << "Number of tasks: ";
    std::cin >> num_tasks;

    if (num_tasks <= 0) {
        std::cerr << "Number of tasks must be greater than 0\n";
        exit(1);
    }

    array_list* list = create_array_list(DEFAULT_CAPACITY);
    int pid, priority, cycles;
    std::unordered_set<int> pids;

    for (int i = 0; i < num_tasks; i++) {
        std::cout << "Enter task " << i
                  << "'s pid, priority, cycles (space-separated): ";
        std::cin >> pid;
        if (pids.find(pid) != pids.end()) {
            std::cerr << "Duplicate pid " << pid << " entered\n";
            exit(1);
        }
        pids.emplace(pid);
        std::cin >> priority;
        std::cin >> cycles;
        task_struct* task = new task_struct;
        task->pid = pid;
        task->priority = priority;
        task->remaining_cycles = cycles;
        if (scheduling_algo == PRQUEUE)
            enqueue(
                list,
                task);  // inefficient because this is successive
                        // enqueues O(n * log(n)), slower than the O(n) build
                        // heap from array algorithm, but this is simple
        else
            add(list, task);
    }

    std::cout << "\n";
    print_table_as_grid(array_list_to_table(list));
    std::cout << "\n";

    switch (scheduling_algo) {
        case FCFS: {
            fcfs(list);
            break;
        }
        case RR: {
            round_robin(list, QUANTUM);
            break;
        }
        case PRQUEUE: {
            priority_queue(list, QUANTUM);
            break;
        }
        default:  // Unreachable
            break;
    }
}
