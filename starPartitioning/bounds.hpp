#pragma once
#include "../graph.hpp"

/**
 * @brief calculates an a tribial upperbound for number of stars needed when Vertex cover is given
 * calculates in polynomial time
 */
int getUB(const Graph &g, const std::vector<vertex> &vc);

int getLB(const Graph &g);