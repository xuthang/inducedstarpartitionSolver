#pragma once

#include "../graph.hpp"
#include "starPartitions.h"

bool isValidStarPartition(const Graph &g, const StarPartitions &S);
bool isValidStarPartition(const Graph &g, const StarPartitions &S, int solutionSize);
