/**
 *   Created by mbenlioglu on Oct 03, 2020.
 */
#pragma once
#include <iostream>
#include "graph.hpp"

/**
 * @brief Constructor to create Graph object from a file
 * @tparam alg      which algorithm to be used in MST calculation
 * @param filename  File that contains the adjacency list representation of graph
 */
template<Algorithm alg>
Graph<alg>::Graph(std::string &filename) {
    std::ifstream gfile(filename);
    if (gfile.fail()) throw std::runtime_error("Cannot open file: " + filename);
    gfile >> numVertices >> numEdges;
    edgeList.reserve(numEdges);
    for (unsigned i = 0; i < numEdges; ++i) {
        Edge e{};
        gfile >> e.src >> e.dest >> e.weight;
        edgeList.push_back(e);
    }


    if constexpr (alg == KRUSKAL) {
        edgeHeap.reserve(edgeList.size());
        edgeHeap.assign(edgeList.begin(), edgeList.end());
        // Min heap based on edge weights
        std::make_heap(edgeHeap.begin(), edgeHeap.end(), [](const Edge left, const Edge right) {
            return left.weight > right.weight;
        });
        MST.reserve(numVertices);
    } else {

    }
}

/**
 * @brief Constructor to create Graph object from a file
 * @tparam alg      which algorithm to be used in MST calculation
 * @param filename  File that contains the adjacency list representation of graph
 */
template<Algorithm alg>
Graph<alg>::Graph(const char *filename) : Graph(reinterpret_cast<std::basic_string<char> &>(filename)) {}

/**
 * @brief Calculate MST of the entire graph by either Kruskal's or Prim's based on the
 * initialization of the graph
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::computeMST() {
    if constexpr (alg == KRUSKAL) return this->kruskalMST();
    else return this->primsMST();
}

/**
 * @brief Update the existing MST with the additional edge provided.
 * @param e     Edge to be added
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::recomputeMST(Edge e) {
    auto start = clock::now();

    Edge maxEdge = e;
    // Find the unique path between e.src and e.dest, get the max edge along the path
    do {
        auto v = MST.find(maxEdge.src);
        if (v == MST.end()) v = MST.find(maxEdge.dest);
        if (v == MST.end()) throw std::runtime_error("Invalid Edge!");
        if (v->second.second > maxEdge.weight) maxEdge = {v->first, v->second.first, v->second.second};
    } while (v->first == e.dest);

    // If the max edge along the path is greater than the new edge, replace.
    if (maxEdge.weight > e.weight) {
        MST.erase(maxEdge.src);
        MST.insert({e.src, {e.dest, e.weight}});
        mstWeight -= maxEdge.weight - e.weight;
    }

    return {mstWeight, duration(clock::now() - start).count()};
}

//=================================================================================================
// Kruskal's

/**
 * @brief Calculate MST of the graph using Kruskal's algorithm
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::kruskalMST() {
    std::vector<Edge> inserted(numEdges);
    std::unordered_set<unsigned> added;
    added.reserve(numVertices);

    auto start = clock::now();

    while (!edgeHeap.empty()) {
        // Get the edge with min weight
        Edge e = edgeHeap.front();

        // If both vertices connected by this edge is already in the MST, discard
        if (added.find(e.src) == added.end() || added.find(e.dest) == added.end()) {
            added.insert(e.src);
            added.insert(e.dest);
            inserted.push_back(e);
        }

        // Pop the edge from heap
        std::pop_heap(edgeHeap.begin(), edgeHeap.end(), [](const Edge left, const Edge right) {
            return left.weight > right.weight;
        });
        edgeHeap.pop_back();
    }

    // Construct hashmap from inserted edges
    for (auto &e: inserted) {
        if (MST.find(e.src) == MST.end())
            MST[e.src] = {e.dest, e.weight};
        else
            MST[e.dest] = {e.src, e.weight};
    }

    return {mstWeight, duration(clock::now() - start).count()};
}

//=================================================================================================
// Prim's

/**
 * @brief Calculate MST of the graph using Prim's algorithm
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::primsMST() {
    throw std::logic_error("Not yet implemented");
}
