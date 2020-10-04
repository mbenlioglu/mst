/**
 *   Created by mbenlioglu on Oct 03, 2020.
 */

#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <queue>
#include <unordered_map>


struct perfData {
    int mstWeight;
    double duration;
};

enum Algorithm {
    KRUSKAL, PRIM
};

struct Edge {
    unsigned src, dest, weight;
};

template<Algorithm alg = KRUSKAL>
class Graph {
private:
    // Timing
    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<double, std::milli>;
    unsigned numVertices{}, numEdges{};

    // Functions
    perfData kruskalMST();
    perfData recomputeKruskalMST(Edge e);
    perfData primsMST();
    perfData recomputePrimsMST(Edge e);
public:
    Graph() = default;
    explicit Graph(std::string &filename);
    explicit Graph(const char *filename);

    perfData computeMST();
    perfData recomputeMST(Edge e);
};

#include "graph.cpp"
