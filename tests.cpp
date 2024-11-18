#include "tests.hpp"

#include <vector>

#include "array_list.h"
#include "main.cpp"
#include "min_heap.h"
#include "task.h"

const int FCFS_POINTS_PER_TEST_CASE = 1;
const int RR_POINTS_PER_TEST_CASE = 1;
const int PRQUEUE_POINTS_PER_TEST_CASE = 4;
const unsigned long BUFSIZE_FOR_CAPTURING_STDOUT = 1 << 16;

class CommonTestData : public ::testing::Test {
   protected:
    array_list* insertions_cycles_0;
    array_list* insertions_cycles_nonzero;

    void SetUp() override {
        insertions_cycles_0 = new array_list;
        insertions_cycles_0->size = 8;
        insertions_cycles_0->capacity = 10;
        insertions_cycles_0->array =
            new task_struct* [insertions_cycles_0->capacity] {
                new task_struct{0, 17, 0}, new task_struct{1, 14, 0},
                    new task_struct{2, 21, 0}, new task_struct{3, 63, 0},
                    new task_struct{4, 12, 0}, new task_struct{5, 19, 0},
                    new task_struct{6, 40, 0}, new task_struct{7, 1, 0},
            };

        insertions_cycles_nonzero = new array_list;
        insertions_cycles_nonzero->size = 7;
        insertions_cycles_nonzero->capacity = 10;
        insertions_cycles_nonzero->array =
            new task_struct* [insertions_cycles_nonzero->capacity] {
                new task_struct{1, 5, 10}, new task_struct{2, 16, 17},
                    new task_struct{3, 8, 24}, new task_struct{4, 1, 36},
                    new task_struct{5, 99, 100}, new task_struct{6, 1, 62},
                    new task_struct{0, 26, 80},
            };
    }

    void TearDown() override {}
};

/**
 * This checks whether the output of ./main fcfs < data/in*.txt is the same as
 * data/fcfs_out*.txt
 */
TEST(FCFS, RunMainFuncOnInputFilesInDataDirAndCompareOutput) {
    // Doesn't have to be mmapped because the child doesn't write to this, only
    // reads
    std::vector<std::string> input_files;
    // Must be mmapped because the child writes to this variable
    // Without mmap, the child would operate on its copy of this, so the parent
    // wouldn't see the modifcation
    int* shared_pass = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (const fs::directory_entry& dir_entry :
         fs::recursive_directory_iterator(DATA_DIR))
        if (dir_entry.is_regular_file() &&
            dir_entry.path().string().find("in") != std::string::npos)
            input_files.push_back(dir_entry.path().string());
    RecordProperty("total", size(input_files) * FCFS_POINTS_PER_TEST_CASE);
    run_with_signal_catching([&input_files, &shared_pass]() {
        const int argc = 2;
        for (auto& input_file : input_files) {
            const char* argv[] = {"./main", "fcfs", NULL};

            // Redirect stdin from input file
            int input_fd = open(input_file.c_str(), O_RDONLY);
            int original_stdin_fd = dup(STDIN_FILENO);
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);

            // Since _main prints to stdout, capture stdout
            int pipe_fd[2];
            pipe(pipe_fd);
            int original_stdout_fd = dup(STDOUT_FILENO);
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);

            _main(argc, argv);

            fflush(stdout);
            dup2(original_stdout_fd, STDOUT_FILENO);
            close(original_stdout_fd);

            char buffer[BUFSIZE_FOR_CAPTURING_STDOUT];
            ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            close(pipe_fd[0]);

            std::string captured_stdout = buffer;
            std::string expected_output_path = input_file;
            size_t pos = expected_output_path.find("in");
            if (pos != std::string::npos)
                expected_output_path.replace(pos, 2, "fcfs_out");
            std::ifstream expected_output_file(expected_output_path);
            std::string expected(
                (std::istreambuf_iterator<char>(expected_output_file)),
                std::istreambuf_iterator<char>());
            std::string actual = captured_stdout;
            if (expected.compare(actual) == 0)
                *shared_pass += FCFS_POINTS_PER_TEST_CASE;

            EXPECT_STREQ(expected.c_str(), actual.c_str())
                << "after running _main with argc " << argc << ", argv {\""
                << argv[0] << "\", \"" << argv[1] << "\"}, and stdin from "
                << input_file;

            // Restore stdin
            dup2(original_stdin_fd, STDIN_FILENO);
            close(original_stdin_fd);
        }
    });
    RecordProperty("pass", *shared_pass);
    munmap(shared_pass, sizeof(int));
}

/**
 * This checks whether the output of ./main rr < data/in*.txt is the same as
 * data/fcfs_out*.txt
 */
TEST(RR, RunMainFuncOnInputFilesInDataDirAndCompareOutput) {
    // Doesn't have to be mmapped because the child doesn't write to this, only
    // reads
    std::vector<std::string> input_files;
    // Must be mmapped because the child writes to this variable
    // Without mmap, the child would operate on its copy of this, so the parent
    // wouldn't see the modifcation
    int* shared_pass = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (const fs::directory_entry& dir_entry :
         fs::recursive_directory_iterator(DATA_DIR))
        if (dir_entry.is_regular_file() &&
            dir_entry.path().string().find("in") != std::string::npos)
            input_files.push_back(dir_entry.path().string());
    RecordProperty("total", size(input_files) * RR_POINTS_PER_TEST_CASE);
    run_with_signal_catching([&input_files, &shared_pass]() {
        const int argc = 2;
        for (auto& input_file : input_files) {
            const char* argv[] = {"./main", "rr", NULL};

            // Redirect stdin from input file
            int input_fd = open(input_file.c_str(), O_RDONLY);
            int original_stdin_fd = dup(STDIN_FILENO);
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);

            // Since _main prints to stdout, capture stdout
            int pipe_fd[2];
            pipe(pipe_fd);
            int original_stdout_fd = dup(STDOUT_FILENO);
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);

            _main(argc, argv);

            fflush(stdout);
            dup2(original_stdout_fd, STDOUT_FILENO);
            close(original_stdout_fd);

            char buffer[BUFSIZE_FOR_CAPTURING_STDOUT];
            ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            close(pipe_fd[0]);

            std::string captured_stdout = buffer;
            std::string expected_output_path = input_file;
            size_t pos = expected_output_path.find("in");
            if (pos != std::string::npos)
                expected_output_path.replace(pos, 2, "rr_out");
            std::ifstream expected_output_file(expected_output_path);
            std::string expected(
                (std::istreambuf_iterator<char>(expected_output_file)),
                std::istreambuf_iterator<char>());
            std::string actual = captured_stdout;
            if (expected.compare(actual) == 0)
                *shared_pass += RR_POINTS_PER_TEST_CASE;

            EXPECT_STREQ(expected.c_str(), actual.c_str())
                << "after running _main with argc " << argc << ", argv {\""
                << argv[0] << "\", \"" << argv[1] << "\"}, and stdin from "
                << input_file;

            // Restore stdin
            dup2(original_stdin_fd, STDIN_FILENO);
            close(original_stdin_fd);
        }
    });
    RecordProperty("pass", *shared_pass);
    munmap(shared_pass, sizeof(int));
}

SAFE_TEST_F(CommonTestData, EnqueueCyclesZero, {
    array_list* actual = create_array_list(DEFAULT_CAPACITY);
    for (int i = 0; i < insertions_cycles_0->size; i++) {
        enqueue(actual, insertions_cycles_0->array[i]);
    }

    EXPECT_EQ(insertions_cycles_0->size, actual->size);

    EXPECT_EQ(1, actual->array[0]->priority);
    EXPECT_EQ(12, actual->array[1]->priority);
    EXPECT_EQ(19, actual->array[2]->priority);
    EXPECT_EQ(14, actual->array[3]->priority);
    EXPECT_EQ(17, actual->array[4]->priority);
    EXPECT_EQ(21, actual->array[5]->priority);
    EXPECT_EQ(40, actual->array[6]->priority);
    EXPECT_EQ(63, actual->array[7]->priority);
})

SAFE_TEST_F(CommonTestData, EnqueueCyclesNonzero, {
    array_list* actual = create_array_list(DEFAULT_CAPACITY);
    for (int i = 0; i < insertions_cycles_nonzero->size; i++) {
        enqueue(actual, insertions_cycles_nonzero->array[i]);
    }

    EXPECT_EQ(insertions_cycles_nonzero->size, actual->size);

    EXPECT_EQ(6, actual->array[0]->pid);
    EXPECT_EQ(1, actual->array[0]->priority);
    EXPECT_EQ(62, actual->array[0]->remaining_cycles);

    EXPECT_EQ(3, actual->array[1]->pid);
    EXPECT_EQ(8, actual->array[1]->priority);
    EXPECT_EQ(24, actual->array[1]->remaining_cycles);

    EXPECT_EQ(4, actual->array[2]->pid);
    EXPECT_EQ(1, actual->array[2]->priority);
    EXPECT_EQ(36, actual->array[2]->remaining_cycles);

    EXPECT_EQ(2, actual->array[3]->pid);
    EXPECT_EQ(16, actual->array[3]->priority);
    EXPECT_EQ(17, actual->array[3]->remaining_cycles);

    EXPECT_EQ(5, actual->array[4]->pid);
    EXPECT_EQ(99, actual->array[4]->priority);
    EXPECT_EQ(100, actual->array[4]->remaining_cycles);

    EXPECT_EQ(1, actual->array[5]->pid);
    EXPECT_EQ(5, actual->array[5]->priority);
    EXPECT_EQ(10, actual->array[5]->remaining_cycles);

    EXPECT_EQ(0, actual->array[6]->pid);
    EXPECT_EQ(26, actual->array[6]->priority);
    EXPECT_EQ(80, actual->array[6]->remaining_cycles);
})

SAFE_TEST(Dequeue, CyclesZero, {
    array_list* min_heap = create_array_list(DEFAULT_CAPACITY);
    min_heap->size = 8;
    min_heap->array = new task_struct*[8];
    min_heap->array[0] = new task_struct({0, 1, 0});
    min_heap->array[1] = new task_struct({1, 12, 0});
    min_heap->array[2] = new task_struct({2, 19, 0});
    min_heap->array[3] = new task_struct({3, 14, 0});
    min_heap->array[4] = new task_struct({4, 17, 0});
    min_heap->array[5] = new task_struct({5, 21, 0});
    min_heap->array[6] = new task_struct({6, 40, 0});
    min_heap->array[7] = new task_struct({7, 63, 0});

    EXPECT_EQ(1, dequeue(min_heap)->priority);
    EXPECT_EQ(7, min_heap->size);

    EXPECT_EQ(12, dequeue(min_heap)->priority);
    EXPECT_EQ(6, min_heap->size);

    EXPECT_EQ(14, dequeue(min_heap)->priority);
    EXPECT_EQ(5, min_heap->size);

    EXPECT_EQ(17, dequeue(min_heap)->priority);
    EXPECT_EQ(4, min_heap->size);

    EXPECT_EQ(19, dequeue(min_heap)->priority);
    EXPECT_EQ(3, min_heap->size);

    EXPECT_EQ(21, dequeue(min_heap)->priority);
    EXPECT_EQ(2, min_heap->size);

    EXPECT_EQ(40, dequeue(min_heap)->priority);
    EXPECT_EQ(1, min_heap->size);

    EXPECT_EQ(63, dequeue(min_heap)->priority);
    EXPECT_EQ(0, min_heap->size);
})

SAFE_TEST(Dequeue, CyclesNonzero, {
    array_list* min_heap = create_array_list(DEFAULT_CAPACITY);
    min_heap->size = 7;
    min_heap->array = new task_struct*[7];
    min_heap->array[0] = new task_struct({6, 1, 62});
    min_heap->array[1] = new task_struct({3, 8, 24});
    min_heap->array[2] = new task_struct({4, 1, 36});
    min_heap->array[3] = new task_struct({2, 16, 17});
    min_heap->array[4] = new task_struct({5, 99, 100});
    min_heap->array[5] = new task_struct({1, 5, 10});
    min_heap->array[6] = new task_struct({0, 26, 80});

    task_struct* min = dequeue(min_heap);
    EXPECT_EQ(6, min->pid);
    EXPECT_EQ(1, min->priority);
    EXPECT_EQ(62, min->remaining_cycles);
    EXPECT_EQ(6, min_heap->size);

    min = dequeue(min_heap);
    EXPECT_EQ(4, min->pid);
    EXPECT_EQ(1, min->priority);
    EXPECT_EQ(36, min->remaining_cycles);
    EXPECT_EQ(5, min_heap->size);

    min = dequeue(min_heap);
    EXPECT_EQ(3, min->pid);
    EXPECT_EQ(8, min->priority);
    EXPECT_EQ(24, min->remaining_cycles);
    EXPECT_EQ(4, min_heap->size);

    min = dequeue(min_heap);
    EXPECT_EQ(0, min->pid);
    EXPECT_EQ(26, min->priority);
    EXPECT_EQ(80, min->remaining_cycles);
    EXPECT_EQ(3, min_heap->size);

    min = dequeue(min_heap);
    EXPECT_EQ(1, min->pid);
    EXPECT_EQ(5, min->priority);
    EXPECT_EQ(10, min->remaining_cycles);
    EXPECT_EQ(2, min_heap->size);

    min = dequeue(min_heap);
    EXPECT_EQ(2, min->pid);
    EXPECT_EQ(16, min->priority);
    EXPECT_EQ(17, min->remaining_cycles);
    EXPECT_EQ(1, min_heap->size);

    min = dequeue(min_heap);
    EXPECT_EQ(5, min->pid);
    EXPECT_EQ(99, min->priority);
    EXPECT_EQ(100, min->remaining_cycles);
    EXPECT_EQ(0, min_heap->size);
})

/**
 * This checks whether the output of ./main prqueue < data/in*.txt is the same
 * as data/fcfs_out*.txt
 */
TEST(PrQueue, RunMainFuncOnInputFilesInDataDirAndCompareOutput) {
    // Doesn't have to be mmapped because the child doesn't write to this, only
    // reads
    std::vector<std::string> input_files;
    // Must be mmapped because the child writes to this variable
    // Without mmap, the child would operate on its copy of this, so the parent
    // wouldn't see the modifcation
    int* shared_pass = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (const fs::directory_entry& dir_entry :
         fs::recursive_directory_iterator(DATA_DIR))
        if (dir_entry.is_regular_file() &&
            dir_entry.path().string().find("in") != std::string::npos)
            input_files.push_back(dir_entry.path().string());
    RecordProperty("total", size(input_files) * PRQUEUE_POINTS_PER_TEST_CASE);
    run_with_signal_catching([&input_files, &shared_pass]() {
        const int argc = 2;
        for (auto& input_file : input_files) {
            const char* argv[] = {"./main", "prqueue", NULL};

            // Redirect stdin from input file
            int input_fd = open(input_file.c_str(), O_RDONLY);
            int original_stdin_fd = dup(STDIN_FILENO);
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);

            // Since _main prints to stdout, capture stdout
            int pipe_fd[2];
            pipe(pipe_fd);
            int original_stdout_fd = dup(STDOUT_FILENO);
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);

            _main(argc, argv);

            fflush(stdout);
            dup2(original_stdout_fd, STDOUT_FILENO);
            close(original_stdout_fd);

            char buffer[BUFSIZE_FOR_CAPTURING_STDOUT];
            ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
            buffer[bytes_read] = '\0';
            close(pipe_fd[0]);

            std::string captured_stdout = buffer;
            std::string expected_output_path = input_file;
            size_t pos = expected_output_path.find("in");
            if (pos != std::string::npos)
                expected_output_path.replace(pos, 2, "prqueue_out");
            std::ifstream expected_output_file(expected_output_path);
            std::string expected(
                (std::istreambuf_iterator<char>(expected_output_file)),
                std::istreambuf_iterator<char>());
            std::string actual = captured_stdout;
            if (expected.compare(actual) == 0)
                *shared_pass += PRQUEUE_POINTS_PER_TEST_CASE;

            EXPECT_STREQ(expected.c_str(), actual.c_str())
                << "after running _main with argc " << argc << ", argv {\""
                << argv[0] << "\", \"" << argv[1] << "\"}, and stdin from "
                << input_file;

            // Restore stdin
            dup2(original_stdin_fd, STDIN_FILENO);
            close(original_stdin_fd);
        }
    });
    RecordProperty("pass", *shared_pass);
    munmap(shared_pass, sizeof(int));
}
