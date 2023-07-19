#include <bits/stdc++.h>
#include "gtest/gtest.h"
#include "../graph.hpp"
#include "../starPartitioning/inducedStarPartition.hpp"
#include "../starPartitioning/validator.hpp"
#include <random>

#include <set>
using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST(correctSize, oneVertex)
{
	Graph g(1);
	StarPartitions S;
	EXPECT_EQ(1, inducedStarPartitioning_vc(g, {}, S));
}

TEST(correctSize, K2)
{
	Graph g(2);
	g.addEdge(g.getVertices()[0], g.getVertices()[1]);
	StarPartitions S;
	EXPECT_EQ(1, inducedStarPartitioning_vc(g, {g.getVertices()[0]}, S));
}

TEST(correctSize, bigStar)
{
	Graph g(9);
	auto V = g.getVertices();
	for(int i = 1; i < (int)g.getVertexcount(); i++)
		g.addEdge(V[0], V[i]);

	StarPartitions S;
	EXPECT_EQ(1, inducedStarPartitioning_vc(g, {V[0]}, S));
}

TEST(correctSize, multipleComponents)
{
	Graph g(10);
	auto V = g.getVertices();
	std::vector<vertex> VC;

	VC.push_back(V[0]);
	for(int i = 0+1; i < 0+4; i++)
		g.addEdge(V[0], V[i]);

	VC.push_back(V[4]);
	for(int i = 4+1; i < 4+3; i++)
		g.addEdge(V[4], V[i]);

	VC.push_back(V[7]);
	for(int i = 7+1; i < (int)V.size(); i++)
		g.addEdge(V[7], V[i]);

	StarPartitions S;
	EXPECT_EQ(3, inducedStarPartitioning_vc(g, VC, S));
}

TEST(ComplexGraph, C5)
{
	Graph g(5);
	auto V = g.getVertices();
	for(int i = 0; i < 5 ; i++)
	{
		vertex u = V[i], v = V[(i+1)%V.size()];
		g.addEdge(u, v);
	}
	std::vector<vertex> VC = {V[1], V[3]};

	StarPartitions S;
	EXPECT_EQ(2, inducedStarPartitioning_vc(g, VC, S));
}

TEST(validatorCheck, validStar)
{
	Graph g(4);
	auto V = g.getVertices();
	vertex c = V[1];
	std::set<vertex> l;
	for(auto v : V)
	{
		if(v != c)
		{
			g.addEdge(v, c);
			l.insert(v);
		}
	}

	StarPartitions S;
	EXPECT_EQ(1, inducedStarPartitioning_vc(g, {c}, S));

	EXPECT_EQ(S.partitions.size(), S.size());
	EXPECT_EQ(S.partitions.size(), 1);

	auto S1 = S.partitions[0];
	EXPECT_EQ(S1.getCenter(), c);
	EXPECT_EQ(S1.getLeaves(), l);
}
