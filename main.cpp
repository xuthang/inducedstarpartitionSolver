#include <iostream>
#include <cassert>

#include "utility/IO.hpp"
#include "utility/configuration.h"
#include "utility/timer.hpp"
#include "graph.hpp"

#include "starPartitioning/starPartitions.h"
#include "starPartitioning/inducedStarPartition.hpp"
#include "starPartitioning/validator.hpp"

#include "externalSolvers/vertexCoverSolver.hpp"

#include "utility/debug.hpp"

int main(int argc, char **argv)
{
	configuration config = __defaultConfig;
	config = loadConfig(argc, argv);
	Graph g = loadGraph(config);

	StarPartitions S;
	int solutionSize;
	// solutionSize = getTrivialSolution(g, S);
	// assert(isValidStarPartition(g, S));

	std::vector<vertex> vc;
	if (config.hasDefinedVC())
	{
		vc = loadVC(config);
	}
	else
	{
		vc = getVertexCover(g);
	}
	std::cerr << "VC: " << vc.size() << std::endl;

	StarPartitions S_vc;
	solutionSize = inducedStarPartitioning_vc(g, vc, S_vc);

#ifdef __DEBUG__
	assert(solutionSize != -1);
	assert(isValidStarPartition(g, S_vc));
#endif

	printSolution(std::cout, g, S_vc);
}
