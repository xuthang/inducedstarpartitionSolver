#pragma once 

#include "../graph.hpp"
#include "../starPartitioning/starPartitions.h"
#include "configuration.h"
#include <iostream>

configuration loadConfig(int argc, char ** argv);

Graph loadGraph(const configuration & config);

std::ostream & printGraph(std::ostream &out, const Graph & g);

std::ostream & printSolution(std::ostream &out, const Graph &g, const StarPartitions & S);

std::vector<vertex> loadVC(const configuration & config);