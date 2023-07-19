#pragma once

#include "../graph.hpp"
#include "starPartitions.h"
#include <vector>

/**
 * @param S the resulting partitioning of G
 * @return int min number of stars needed
 */
int inducedStarPartitioning_vc(const Graph & g, const std::vector<vertex> & vc, StarPartitions &S);

/**
 * @brief Get the Trivial Solution for g in polynomial time, is not guaranteed to be optimal
 * @return int number of stars needed
 */
int getTrivialSolution(const Graph & g, StarPartitions &S);