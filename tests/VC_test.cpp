#include <bits/stdc++.h>
#include "gtest/gtest.h"
#include "../graph.hpp"
#include "../externalSolvers/vertexCoverSolver.hpp"
#include "../starPartitioning/genericAlgorithms.hpp"
#include <random>

#include <set>
using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST(smallTest, independentSet)
{
	Graph g(10);
	auto sol = getVertexCover(g);
	ASSERT_EQ(sol.size(), 0);

	Graph g1(1);
	auto sol1 = getVertexCover(g1);
	ASSERT_EQ(sol1.size(), 0);

	Graph g2(1000);
	auto sol2 = getVertexCover(g2);
	ASSERT_EQ(sol2.size(), 0);
}

TEST(smallTest, tree1)
{
	Graph g(2);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	auto sol = getVertexCover(g);
	ASSERT_EQ(sol.size(), 1);
}

TEST(smallTest, tree2)
{
	Graph g(5);
	auto V = g.getVertices();

	for (int i = 0; i + 1 < 5; i++)
		g.addEdge(V[i], V[i + 1]);

	auto sol = getVertexCover(g);
	ASSERT_EQ(sol.size(), 2);

	for (auto v : sol)
		g.removeVertex(v);

	ASSERT_EQ(g.getEdgecount(), 0);
}

TEST(smallTest, tree3)
{
	Graph g((1 << 5) - 1);
	auto V = g.getVertices();

	// root node V[0] does not have a parent edge
	for (int i = 1; i < (int)g.getVertexcount(); i++)
		g.addEdge(V[i], V[i / 2]); // parent edge

	ASSERT_EQ(g.getEdgecount(), g.getVertexcount() - 1);
	auto sol = getVertexCover(g);

	for (auto v : sol)
		g.removeVertex(v);

	ASSERT_EQ(g.getEdgecount(), 0);
	ASSERT_TRUE(isIndependentSet(g.getVertices(), g));
}

TEST(randomgraphs, random1)
{
	Graph g(50);
	auto V = g.getVertices();
	srand(100);

	int ecount = 100 + (rand() % 600);
	for (int i = 0; i < ecount; i++)
	{
		int u = rand() % V.size(), v = rand() % V.size();
		if (u != v)
			g.addEdge(V[u], V[v]);
	}

	auto sol = getVertexCover(g);

	for (auto v : sol)
		g.removeVertex(v);

	ASSERT_EQ(g.getEdgecount(), 0);
	ASSERT_TRUE(isIndependentSet(g.getVertices(), g));
}