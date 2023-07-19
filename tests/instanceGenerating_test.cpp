#include <bits/stdc++.h>
#include "gtest/gtest.h"
#include "../graph.hpp"
#include "../starPartitioning/VC_solution.hpp"
#include "../utility/instanceGenerators.hpp"
#include "../externalSolvers/vertexCoverSolver.hpp"

#include <set>
using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST(smallG, compilationTest)
{
	Graph g = generateGraph_VC(10, 3, 0.5, 6);
	ASSERT_EQ(g.getVertexcount(), 10);

	auto sol = getVertexCover(g);
	ASSERT_LE(sol.size(), 3);
}

TEST(bigG, checkEdgeRate)
{
	int instancesWithTooManyEdges = 0;
	int n = 500, maxE = n * (n - 1) / 2;

	for (int i = 0; i < 1000; ++i)
	{
		Graph g = generateGraph_VC(500, 30, 0.4, i * 3 + 51848);
		if (g.getEdgecount() > maxE * 0.4)
			instancesWithTooManyEdges++;
	}
	// we dont want to create graphs with too many edges too often, some randomness is allowed and can sometime generate a lot of edges
	// but this should not happen too often
	ASSERT_LE(instancesWithTooManyEdges, 1000 * 0.3);
}
