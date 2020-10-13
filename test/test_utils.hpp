/**
 *   Created by mbenlioglu on Oct 12, 2020.
 */

#pragma once

#include <iostream>

inline void print_usage(std::string &basename) {
    std::cout << "Usage:\n" << basename << " GRAPH_FILE CHANGE_FILE OUTPUT_FILE\n"
              << "\n\t GRAPH_FILE \t Path to graph file (edge list format)"
                 "\n\t CHANGE_FILE\t Path to file containing additional edges"
                 "\n\t RESULT_FILE\t Path to results for comparison"
              << std::endl;
}

#define FILE_FAIL(fname)                                            \
    do {                                                            \
        std::cerr << "Cannot open file: " << fname << std::endl;    \
    } while(0)

#define ASSERTEQ(x, y)                                                                  \
    do {                                                                                \
        if ((x) != (y)) {                                                               \
            std::cerr << "TEST FAILURE: Expected: " << y << " got: " << x               \
                << " for: " << #x << " " << __FILE__ << ":" << __LINE__ << std::endl;   \
            return EXIT_FAILURE;                                                        \
        }                                                                               \
    } while(0)

#define ASSERTNEQ(x, y)                                                                 \
    do {                                                                                \
        if ((x) == (y)) {                                                               \
            std::cerr << "TEST FAILURE: Expected: " << y << " got: " << x               \
                << " for: " << #x << " " << __FILE__ << ":" << __LINE__ << std::endl;   \
            return EXIT_FAILURE;                                                        \
        }                                                                               \
    } while(0)

#define RUN_TEST(fn)                                    \
    do {                                                \
        int ret_ = fn();                                \
        if (ret_ != 0) {                                \
            fprintf(stderr, "TEST FAILED: %s\n", #fn);  \
        } else {                                        \
            fprintf(stdout, "TEST PASSED: %s\n", #fn);  \
        }                                               \
            ret |= ret_;                                \
    } while (0)

#define RUN_TEST_ARGS(fn, ...)                          \
    do {                                                \
        int ret_ = fn(__VA_ARGS__);                     \
        if (ret_ != 0) {                                \
            fprintf(stderr, "TEST FAILED: %s\n", #fn);  \
        } else {                                        \
            fprintf(stdout, "TEST PASSED: %s\n", #fn);  \
        }                                               \
            ret |= ret_;                                \
    } while (0)
