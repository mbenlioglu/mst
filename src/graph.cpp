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


    MST.resize(numVertices);
    if constexpr (alg == KRUSKAL) {
        edgeList.reserve(numEdges);
        for (unsigned i = 0; i < numEdges; ++i) {
            Edge e{};
            gfile >> e.src >> e.dest >> e.weight;
            edgeList.push_back(e);
        }
        edgeHeap.reserve(edgeList.size());
        edgeHeap.assign(edgeList.begin(), edgeList.end());
        // Min heap based on edge weights
        std::make_heap(edgeHeap.begin(), edgeHeap.end(), [](const Edge left, const Edge right) {
            return left.weight > right.weight;
        });
    } else {
        adjList.reserve(numVertices);
        for (unsigned i = 0; i < numEdges; ++i) {
            Edge e{};
            gfile >> e.src >> e.dest >> e.weight;
            adjList[e.src].push_back(e);
            adjList[e.dest].push_back({e.dest, e.src, e.weight});
        }
        adjHeap.reserve(numEdges);
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
 * Find the unique path between u and v vertices in a BFS manner.
 * @param u     Source vertex
 * @param v     Destination vertex
 * @return      Path between source and destination
 */
template<Algorithm alg>
auto Graph<alg>::findPath(unsigned u, unsigned v) {
    std::vector<bool> visited(numVertices);
    std::queue<std::pair<unsigned, unsigned> > q;
    std::vector<std::vector<Edge> > paths(numVertices);

    // Push source vertex to search queue
    q.push({u, std::numeric_limits<unsigned>::max()});
    while (!q.empty()) {
        // Get first element from queue
        auto[parent, dest] = q.front();
        q.pop();

        // mark as visited
        visited[parent] = true;

        // Add neighbors of children to queue
        for (auto &child: MST[parent]) {
            if (!visited[child.first]) {
                q.push({child.first, parent});

                // Update paths
                paths[child.first] = paths[parent];
                paths[child.first].push_back({parent, child.first, child.second});
            }
        }
    }
    return std::move(paths[v]);
}


/**
 * @brief Update the existing MST with the additional edge provided.
 * @param e     Edge to be added
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::recomputeMST(const Edge &e) {
    auto start = clock::now();

    // Find the unique path between e.src and e.dest, get the max edge along the path
    auto path = findPath(e.src, e.dest);

    // If the max edge along the path is greater than the new edge, replace.
    auto maxEdge = std::max_element(path.begin(), path.end(), [](const Edge left, const Edge right) {
        return left.weight < right.weight;
    });
    if (maxEdge->weight > e.weight) {
        // Remove maxEdge from MST
        MST[maxEdge->src].erase(std::find_if(MST[maxEdge->src].begin(), MST[maxEdge->src].end(), [&](const auto el) {
            return el.first == maxEdge->dest;
        }));
        MST[maxEdge->dest].erase(std::find_if(MST[maxEdge->dest].begin(), MST[maxEdge->dest].end(), [&](const auto el) {
            return el.first == maxEdge->src;
        }));

        // Add new edge
        MST[e.src].push_back({e.dest, e.weight});
        MST[e.dest].push_back({e.src, e.weight});

        // Update mstWeight
        mstWeight -= maxEdge->weight - e.weight;
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
    auto start = clock::now();

    std::vector<std::unordered_set<unsigned> > forest(numVertices);
    for (unsigned i = 0; i < numVertices; ++i) forest[i].insert(i);
    auto find_set = [&forest](unsigned i) {
        return std::find_if(forest.begin(), forest.end(), [&](const std::unordered_set<unsigned> &s) {
            return s.find(i) != s.end();
        });
    };

    // MST edges
    while (!edgeHeap.empty()) {
        // Get the edge with min weight
        Edge e = edgeHeap.front();

        // If both vertices connected by this edge is already in the MST, discard
        auto u = find_set(e.src);
        auto v = find_set(e.dest);
        if (u != v) {
            MST[e.src].push_back({e.dest, e.weight});
            MST[e.dest].push_back({e.src, e.weight});
            mstWeight += e.weight;
            u->merge(*v);
            forest.erase(v);
        }

        // Pop the edge from heap
        std::pop_heap(edgeHeap.begin(), edgeHeap.end(), [](const Edge left, const Edge right) {
            return left.weight > right.weight;
        });
        edgeHeap.pop_back();
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
    auto start = clock::now();
    std::vector<bool> visited(numVertices);

    visited[0] = true;
    adjHeap.assign(adjList[0].begin(), adjList[0].end());
    std::make_heap(adjHeap.begin(), adjHeap.end(), [](const Edge left, const Edge right) {
        return left.weight > right.weight;
    });

    while (!adjHeap.empty() || visited.size() != numVertices) {
        std::pop_heap(adjHeap.begin(), adjHeap.end(), [](const Edge left, const Edge right) {
            return left.weight > right.weight;
        });
        Edge e = adjHeap.back();
        adjHeap.pop_back();
        if (!visited[e.dest] || !visited[e.src]) {
            visited[e.dest] = visited[e.src] = true;
            MST[e.src].push_back({e.dest, e.weight});
            MST[e.dest].push_back({e.src, e.weight});
            mstWeight += e.weight;
            for (auto &ed: adjList[e.dest]) {
                adjHeap.push_back(ed);
                std::push_heap(adjHeap.begin(), adjHeap.end(), [](const Edge left, const Edge right) {
                    return left.weight > right.weight;
                });
            }
        }
    }

    return {mstWeight, duration(clock::now() - start).count()};
}
