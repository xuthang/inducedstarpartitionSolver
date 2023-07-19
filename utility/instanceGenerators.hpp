#pragma once
#include "../graph.hpp"


Graph generateGraph_VC(size_t vertexCount, size_t VC_size, double edgeRate);
Graph generateGraph_VC(size_t vertexCount, size_t VC_size, double edgeRate, int seed);

/**
 * @brief generates a graph with vertex cover at most VC_size and returns a vertex cover of size VC_size (there may be a smaller vertex cover)
 * 
 * @param vertexCount size of V(G)
 * @param VC_size max size of min vertex cover
 * @param edgeRate the rate of edges inside the VC, and between VC and outside of VC
 * @param seed graph is generated randomly, use seed to control randomness
 * @param VC return value - a vertex cover G, not necessarily minimal
 * @return Graph 
 */
Graph generateGraph_VC(size_t vertexCount, size_t VC_size, double edgeRate, int seed, std::vector<vertex> & VC);
