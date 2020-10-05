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
#include <unordered_set>


struct perfData {
    int mstWeight;
    double duration;
};

enum Algorithm {
    KRUSKAL, PRIM
};

struct Edge {
    unsigned src, dest;
    int weight;
};

template<Algorithm alg = KRUSKAL>
class Graph {
private:
    // Timing
    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<double>; // in seconds

    unsigned numVertices{}, numEdges{};
    int mstWeight{};
    std::vector<Edge> edgeList;
    std::vector<Edge> edgeHeap;
    std::unordered_map<unsigned, std::pair<unsigned, int> > MST;

    // Prim

    // Functions
    perfData kruskalMST();
    perfData primsMST();
public:
    Graph() = default;
    explicit Graph(std::string &filename);
    explicit Graph(const char *filename);

    perfData computeMST();
    perfData recomputeMST(Edge e);
};

#include "graph.cpp"
