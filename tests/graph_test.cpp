#include <bits/stdc++.h>
#include "gtest/gtest.h"
#include "../graph.hpp"
#include "../starPartitioning/VC_solution.hpp"

#include <set>
using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

//-----------------------------------------------------------------

TEST(smallG, adding)
{
	Graph g;
	ASSERT_EQ(g.getVertexcount(), 0);
	ASSERT_EQ(g.getEdgecount(), 0);

	auto v = g.addVertex();
	ASSERT_EQ(g.getVertexcount(), 1);
	ASSERT_EQ(g.getEdgecount(), 0);
	ASSERT_TRUE(g.hasVertex(v));

	auto u = g.addVertex();
	ASSERT_EQ(g.getVertexcount(), 2);
	ASSERT_EQ(g.getEdgecount(), 0);
	ASSERT_TRUE(g.hasVertex(v));
	ASSERT_TRUE(g.hasVertex(u));

	g.addEdge(u, v);
	ASSERT_EQ(g.getVertexcount(), 2);
	ASSERT_EQ(g.getEdgecount(), 1);
	ASSERT_TRUE(g.hasVertex(v));
	ASSERT_TRUE(g.hasVertex(u));

	auto w = g.addVertex();
	ASSERT_EQ(g.getVertexcount(), 3);
	ASSERT_EQ(g.getEdgecount(), 1);
	ASSERT_TRUE(g.hasVertex(v));
	ASSERT_TRUE(g.hasVertex(u));
	ASSERT_TRUE(g.hasVertex(w));

	g.addEdge(u, v);
	ASSERT_EQ(g.getVertexcount(), 3);
	ASSERT_EQ(g.getEdgecount(), 1);
	ASSERT_TRUE(g.hasVertex(v));
	ASSERT_TRUE(g.hasVertex(u));
	ASSERT_TRUE(g.hasVertex(w));
	ASSERT_TRUE(g.hasEdge(u, v));
	ASSERT_TRUE(g.hasEdge(v, u));

	ASSERT_TRUE(!g.hasEdge(u, w));
	ASSERT_TRUE(!g.hasEdge(w, v));

	g.addEdge(w, v);
	ASSERT_EQ(g.getEdgecount(), 2);
	ASSERT_FALSE(g.hasEdge(u, w));
	ASSERT_TRUE(g.hasEdge(w, v));
	ASSERT_TRUE(g.hasEdge(v, u));

	for (int i = 0; i < 10; i++)
		g.addVertex();

	ASSERT_EQ(g.getVertexcount(), 13);
}

TEST(smallG, neighbourhood)
{
	Graph g(6);
	auto V = g.getVertices();
	ASSERT_EQ(g.getVertexcount(), 6);
	ASSERT_EQ(V.size(), 6);

	g.addEdge(V[0], V[1]);
	g.addEdge(V[0], V[4]);
	g.addEdge(V[5], V[0]);
	g.addEdge(V[1], V[2]);
	g.addEdge(V[3], V[4]);
	ASSERT_EQ(g.getEdgecount(), 5);

	auto N0 = g.getNeighbours(V[0]);
	ASSERT_EQ(N0.size(), 3);
	ASSERT_EQ(set<vertex>(N0.begin(), N0.end()), set<vertex>({V[1], V[4], V[5]}));

	g.addEdge(V[0], V[4]);

	N0 = g.getNeighbours(V[0]);
	ASSERT_EQ(N0.size(), 3);
	ASSERT_EQ(set<vertex>(N0.begin(), N0.end()), set<vertex>({V[1], V[4], V[5]}));

	g.addEdge(V[4], V[2]);
	auto N4 = g.getNeighbours(V[4]);
	ASSERT_EQ(N4.size(), 3);
	ASSERT_EQ(set<vertex>(N4.begin(), N4.end()), set<vertex>({V[0], V[3], V[2]}));
}

TEST(smallG, removeVertex)
{
	Graph g(10);
	auto V = g.getVertices();

	g.addEdge(V[0], V[1]);
	g.addEdge(V[0], V[2]);
	g.addEdge(V[3], V[0]);
	g.addEdge(V[4], V[0]);
	g.addEdge(V[5], V[2]);
	g.addEdge(V[4], V[2]);

	ASSERT_EQ(g.getVertexcount(), 10);
	ASSERT_EQ(g.getEdgecount(), 6);

	g.removeVertex(V[8]);
	ASSERT_EQ(g.getVertexcount(), 9);
	ASSERT_EQ(g.getEdgecount(), 6);

	g.removeVertex(V[6]);
	ASSERT_EQ(g.getVertexcount(), 8);

	g.removeVertex(V[1]);
	ASSERT_EQ(g.getVertexcount(), 7);
	ASSERT_EQ(g.getEdgecount(), 5);

	ASSERT_FALSE(g.hasVertex(V[8]));
	ASSERT_FALSE(g.hasEdge(V[5], V[4]));

	#ifdef __DEBUG__
	EXPECT_THROW(g.hasEdge(V[1], V[0]), std::invalid_argument);
	EXPECT_THROW(g.hasEdge(V[0], V[1]), std::invalid_argument);
	EXPECT_THROW(g.hasEdge(V[8], V[6]), std::invalid_argument);
	#endif

	auto N0 = g.getNeighbours(V[0]);
	ASSERT_EQ(set<vertex>(N0.begin(), N0.end()), set<vertex>({V[2], V[3], V[4]}));

	g.removeVertex(V[0]);
	ASSERT_EQ(g.getEdgecount(), 2);

	#ifdef __DEBUG__
	EXPECT_THROW(g.getNeighbours(V[0]), std::invalid_argument);
	#endif
}

TEST(smallG, removeEdges)
{
	Graph g(8);
	auto V = g.getVertices();

	g.addEdge(V[0], V[1]);
	g.addEdge(V[2], V[1]);
	g.addEdge(V[2], V[3]);
	g.addEdge(V[4], V[3]);

	g.addEdge(V[0], V[5]);
	g.addEdge(V[0], V[3]);

	ASSERT_EQ(g.getEdgecount(), 6);

	g.removeEdge(V[2], V[3]);
	ASSERT_EQ(g.getEdgecount(), 5);
	ASSERT_FALSE(g.hasEdge(V[2], V[3]));

	g.removeEdge(V[1], V[0]);
	ASSERT_EQ(g.getEdgecount(), 4);
	ASSERT_FALSE(g.hasEdge(V[1], V[0]));
	ASSERT_FALSE(g.hasEdge(V[0], V[1]));

	auto N0 = g.getNeighbours(V[0]);
	auto N1 = g.getNeighbours(V[1]);

	ASSERT_EQ(set<vertex>(N0.begin(), N0.end()), set<vertex>({V[5], V[3]}));
	ASSERT_EQ(set<vertex>(N1.begin(), N1.end()), set<vertex>({V[2]}));

	g.removeEdge(V[0], V[5]);
	ASSERT_EQ(g.getEdgecount(), 3);
	auto N5 = g.getNeighbours(V[5]);
	ASSERT_EQ(set<vertex>(N5.begin(), N5.end()), set<vertex>({}));

	ASSERT_TRUE(g.getVertexcount() == 8);
}

TEST(bipartite, test1)
{
	Bipartite b;
	vector<vertex> A = {b.addNewToA(), b.addNewToA(), b.addNewToA()};
	vector<vertex> B = {b.addNewToB(), b.addNewToB()};
	b.addEdge(A.front(), B.front());

	ASSERT_THROW(b.addEdge(A[0], A[1]), std::invalid_argument);
}

TEST(subgraph, onevertex)
{
	Graph g(3);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);

	Graph gsub = g.getInducedSubgraph({V[1]});
	EXPECT_EQ(gsub.getVertices().size(), 1);
	EXPECT_TRUE(gsub.hasVertex(V[1]));
	EXPECT_EQ(gsub.getEdgecount(), 0);

	for(int i = 1; i <= 10; i++)
	{
		gsub.addVertex();
		EXPECT_EQ(gsub.getVertices().size(), i+1);
		EXPECT_TRUE(gsub.hasVertex(V[1]));
	}
}

TEST(subgraph, stillHasEdges)
{
	Graph g(10);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	g.addEdge(V[2], V[1]);
	g.addEdge(V[3], V[1]);
	g.addEdge(V[4], V[1]);
	g.addEdge(V[5], V[1]);

	g.addEdge(V[2], V[0]);
	g.addEdge(V[3], V[4]);

	Graph sub = g.getInducedSubgraph({V[1], V[3], V[4], V[0]});
	EXPECT_EQ(sub.getVertexcount(), 4);
	EXPECT_EQ(sub.getEdgecount(), 4);

	EXPECT_TRUE(sub.hasVertex(V[1]));
	EXPECT_TRUE(sub.hasVertex(V[3]));
	EXPECT_TRUE(sub.hasVertex(V[4]));
	EXPECT_TRUE(sub.hasVertex(V[0]));

	EXPECT_FALSE(sub.hasVertex(V[2]));
	
	for(int i = 1; i <= 100; i++)
		sub.addVertex();
	EXPECT_EQ(sub.getVertexcount(), 4+100);
	EXPECT_EQ(sub.getEdgecount(), 4);
	
}
