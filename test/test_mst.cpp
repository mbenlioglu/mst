/*
 *   Created by mbenlioglu on Oct 12, 2020.
 */

#include "graph.hpp"
#include "test_utils.hpp"

#ifndef ALG

int main (int argc, char *argv[]) {
    std::cerr << "Missing compilation definition! Define ALG=KRUSKAL or ALG=PRIM to determine which\n"
                 "one you want to test." << std::endl;
    return EXIT_FAILURE;
}

#else

template<Algorithm alg>
unsigned test_recompute_mst(Graph<alg> &g, std::ifstream &changes_file, std::ifstream &res_file) {
    unsigned changes_cnt;
    changes_file >> changes_cnt;

    for (unsigned i = 0; i < changes_cnt; ++i) {
        Graph<ALG>::Edge e{};
        changes_file >> e.src >> e.dest >> e.weight;

        // Calculate using the implementation
        perfData p = g.recomputeMST(e);

        // Get results from file
        int resWeight;
        double resTime;
        res_file >> resWeight >> resTime;

        // Compare
        ASSERTEQ(p.mstWeight, resWeight);
    }
    return EXIT_SUCCESS;
}

template<Algorithm alg>
unsigned test_mst(Graph<alg> &g, std::ifstream &res_file) {
    // Calculate using the implementation
    perfData p = g.computeMST();

    // Get results from the file
    int resWeight;
    double resTime;
    res_file >> resWeight >> resTime;

    // Compare
    ASSERTEQ(p.mstWeight, resWeight);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    unsigned ret = EXIT_SUCCESS;

    auto basename = std::string(argv[0]);
    #ifdef _WIN32
    basename = basename.substr(basename.rfind('\\')+1);
    #else
    basename = basename.substr(basename.rfind('/') + 1);
    #endif

    if (argc < 4) {
        print_usage(basename);
        return EXIT_FAILURE;
    }

    // Prepare changes and results files
    std::ifstream changes(argv[2]), results(argv[3]);
    if (changes.fail()) {
        FILE_FAIL(argv[2]);
        return EXIT_FAILURE;
    }
    if (results.fail()) {
        FILE_FAIL(argv[3]);
        changes.close();
        return EXIT_FAILURE;
    }

    try {

        // Create graph object from file
        auto graph = Graph<ALG>(argv[1]);

        RUN_TEST_ARGS(test_mst, graph, results);
        RUN_TEST_ARGS(test_recompute_mst, graph, changes, results);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        ret = EXIT_FAILURE;
    }

    // Clean up
    changes.close();
    results.close();
    return static_cast<int>(ret);
}

#endif /* ALG */
