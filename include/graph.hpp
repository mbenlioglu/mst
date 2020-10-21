/*
 *   Created by mbenlioglu on Oct 03, 2020.
 */

#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <vector>

/**
 * Struct to hold performance metrics (Total MST weight and elapsed time to calculate)
 */
struct perfData {
    int mstWeight;
    double duration;
};

/**
 * List of algorithms used in MST calculation
 */
enum Algorithm {
    KRUSKAL, PRIM
};

template<Algorithm alg = KRUSKAL>
class Graph {
public:
    /**
     * Struct to hold weighted directed edge information
     */
    struct Edge {
        unsigned src, dest;
        int weight;
    };

private:
    // Timing
    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<double>; // in seconds

    unsigned numVertices{}, numEdges{};
    int mstWeight{};
    std::vector<Edge> edgeList;
    std::vector<Edge> edgeHeap;
    std::vector<std::vector<std::pair<unsigned, int> > > MST;

    // Prim
    std::vector<std::vector<Edge> > adjList;
    std::vector<Edge> adjHeap;

    // Functions
    perfData kruskalMST();
    perfData primsMST();
    auto findPath(unsigned u, unsigned v);

public:
    Graph() = default;
    explicit Graph(std::string &filename);
    explicit Graph(const char *filename);

    perfData computeMST();
    perfData recomputeMST(const Edge &e);
};

#include "graph.cpp"
