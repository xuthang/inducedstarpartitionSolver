#include <bits/stdc++.h>
#include "../graph.hpp"
#include "../utility/instanceGenerators.hpp"
#include "../utility/timer.hpp"
#include "../utility/IO.hpp"
#include "../utility/debug.hpp"
#include "../externalSolvers/vertexCoverSolver.hpp"
#include "../starPartitioning/inducedStarPartition.hpp"
#include "../starPartitioning/validator.hpp"
#include <cassert>

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		std::cerr << argv[0] << " "
				  << "[n] [VC size] [edge rate] [optional seed]" << std::endl;
		return 1;
	}

	int n = std::atoi(argv[1]);
	int vc = std::atoi(argv[2]);
	double edge_rate = std::atof(argv[3]);

	int seed = argc >= 5 ? std::atoi(argv[4]) : 123456;


	// for(int i = 0; i < 100; i++)
	{
		// Graph g = generateGraph_VC(n, vc, edge_rate, seed);
		// auto VC = getVertexCover(g);

		std::vector<vertex> VC;
		Graph g = generateGraph_VC(n, vc, edge_rate, seed, VC);

		deb("starting");
		std::cout << g.getVertexcount() << "," << g.getEdgecount() << ",";
		std::cout << edge_rate << "," << VC.size() << "," << std::flush;

		TIMER t;
		StarPartitions S;
		inducedStarPartitioning_vc(g, VC, S);
		// printSolution(std::cout, g, S);
		t.stop(false);

		assert(isValidStarPartition(g, S));
		std::cout << S.size() << "," << t.get() << std::endl;
	}
}