#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

#include "array_list.h"

typedef enum { FCFS, RR, PRQUEUE } scheduling_algo;

/**
 * Prints usage message for main.cpp and exits
 */
void print_usage_and_exit();

/**
 * @note If CLI arg is invalid, prints usage message and exits
 * @param argc
 * @param argv
 * @return scheduling_algo
 */
scheduling_algo parse_cli(int argc, const char* argv[]);

/**
 * Pretty-prints a vector<vector<string>> table
 *
 * @param table m x n vector (all rows are assumed to be the same length)
 */
void print_table_as_grid(std::vector<std::vector<std::string>> table);

/**
 * Converts list to a table format
 *
 * @note Pass the rv to print_table_as_grid
 * @param cache
 * @param num_block_offset_bits
 * @return std::vector<std::vector<std::string>>
 */
std::vector<std::vector<std::string>> array_list_to_table(array_list* list);

/**
 * Runs the scheduling simulation
 *
 * @param scheduling_algo
 */
void sim(scheduling_algo scheduling_algo);

#endif  // UTILS_HPP
