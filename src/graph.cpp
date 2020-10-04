/**
 *   Created by mbenlioglu on Oct 03, 2020.
 */
#pragma once
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
 * @brief Update the existing MST with the additional edge provided by either Kruskal's
 * or Prim's algorithm based on the initialization of the graph
 * @param e     Edge to be added
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::recomputeMST(Edge e) {
    if constexpr (alg == KRUSKAL) return this->recomputeKruskalMST(e);
    else return this->recomputePrimsMST(e);
}

//=================================================================================================
// Kruskal's

/**
 * @brief Calculate MST of the graph using Kruskal's algorithm
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::kruskalMST() {
    return perfData();
}

/**
 * @brief Update the existing MST with an additional edge using Kruskal's algorithm
 * @param e     Edge to be added
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::recomputeKruskalMST(Edge e) {

}

//=================================================================================================
// Prim's

/**
 * @brief Calculate MST of the graph using Prim's algorithm
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::primsMST() {
    return perfData();
}

/**
 * @brief Update the existing MST with and additional edge using Prim's algorithm
 * @param e     Edge to be added
 * @return      perfData struct that has elapsed time and MST weight
 */
template<Algorithm alg>
perfData Graph<alg>::recomputePrimsMST(Edge e) {

}
