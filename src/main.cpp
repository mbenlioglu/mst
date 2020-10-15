/**
 *   Created by mbenlioglu on Sep 24, 2020.
 */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "graph.hpp"


inline void print_usage(std::string &basename) {
    std::cout << "Usage:\n\n" << basename << " GRAPH_FILE CHANGE_FILE OUTPUT_FILE\n"
              << "\n\tGRAPH_FILE \t Path to graph file (edge list format)"
                 "\n\tCHANGE_FILE\t Path to file containing additional edges"
                 "\n\tOUTPUT_FILE\t Path to output results of computation"
              << std::endl;
}

int main(int argc, char *argv[]) {
    auto basename = std::string(argv[0]);
    #ifdef _WIN32
    basename = basename.substr(basename.rfind('\\')+1);
    #else
    basename = basename.substr(basename.rfind('/') + 1);
    #endif

    if (argc < 4) {
        std::cout << "Not enough arguments!" << std::endl;
        print_usage(basename);
        return EXIT_FAILURE;
    }

    // Create graph object from file
    auto graph = Graph<PRIM>(argv[1]);


    // Results storage
    auto perfResults = std::vector<perfData>();


    // Calculate MST from the graph
    perfResults.push_back(graph.computeMST());

    std::ifstream changes(argv[2]);
    if (changes.fail()) {
        std::cerr << "Cannot open file: " << argv[2] << std::endl;
        return EXIT_FAILURE;
    }

    unsigned change_cnt;
    changes >> change_cnt;
    perfResults.reserve(change_cnt + 1);

    for (unsigned i = 0; i < change_cnt; ++i) {
        Edge e{};
        changes >> e.src >> e.dest >> e.weight;

        perfResults.push_back(graph.recomputeMST(e));
    }
    changes.close();

    std::ofstream results(argv[3]);
    if (results.fail()) {
        std::cerr << "Cannot open file: " << argv[3] << std::endl;
        return EXIT_FAILURE;
    }

    for (auto &p: perfResults)
        results << p.mstWeight << "\t" << p.duration << "\n";
    results.close();

    return EXIT_SUCCESS;
}

