#include <bits/stdc++.h>
#include "gtest/gtest.h"
#include "../graph.hpp"
#include "../starPartitioning/genericAlgorithms.hpp"
#include "../starPartitioning/starPartitions.h"
#include <random>
#include "../utility/debug.hpp"

#include <set>
using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST(ISTest, noEdges)
{
	Graph g(5);
	auto V = g.getVertices();
	ASSERT_TRUE(isIndependentSet(V, g));

	ASSERT_TRUE(isIndependentSet({V[3]}, g));
	ASSERT_TRUE(isIndependentSet(vector<vertex>(V.begin(), V.begin() + 2), g));
	ASSERT_TRUE(isIndependentSet(vector<vertex>(V.begin() + 1, V.end()), g));

	ASSERT_TRUE(isIndependentSet({V[3], V[0]}, g));
}

TEST(ISTEST, hasEdges)
{
	Graph g(8);
	auto V = g.getVertices();

	g.addEdge(V[5], V[7]);
	g.addEdge(V[6], V[7]);

	ASSERT_FALSE(isIndependentSet(V, g));
	ASSERT_FALSE(isIndependentSet({V[5], V[7]}, g));

	ASSERT_TRUE(isIndependentSet({V[0], V[1], V[2]}, g));
	ASSERT_TRUE(isIndependentSet({V[0], V[1], V[5]}, g));
	ASSERT_TRUE(isIndependentSet({V[0], V[1], V[5], V[6]}, g));
	ASSERT_TRUE(isIndependentSet({V[5], V[6]}, g));

	ASSERT_FALSE(isIndependentSet({V[7], V[1], V[6]}, g));
}

TEST(starPartitions, IsStarInGraph)
{
	Graph g(5);
	auto V = g.getVertices();
	ASSERT_TRUE(inducesAStar({V[0]}, g));
	ASSERT_TRUE(inducesAStar({V[3]}, g));

	g.addEdge(V[0], V[1]);
	ASSERT_TRUE(inducesAStar({V[0], V[1]}, g));
	ASSERT_TRUE(inducesAStar({V[1], V[0]}, g));

	g.addEdge(V[1], V[2]);
	ASSERT_TRUE(inducesAStar({V[0], V[2], V[1]}, g));
	ASSERT_TRUE(inducesAStar({V[2], V[1], V[0]}, g));

	g.addEdge(V[4], V[1]);
	g.addEdge(V[3], V[1]);

	ASSERT_TRUE(inducesAStar(V, g));
}

TEST(starPartitions, notAStar)
{
	Graph g(7);
	auto V = g.getVertices();

	ASSERT_TRUE(inducesAStar({V[0]}, g));
	ASSERT_FALSE(inducesAStar({V[1], V[0]}, g));

	for (int i = 1; i <= 3; i++)
		g.addEdge(V[0], V[i]);
	vector<vertex> tmp = {V[0], V[1], V[2], V[3]};
	ASSERT_TRUE(inducesAStar(tmp, g));

	auto tmp2 = tmp;
	tmp2.push_back(V[4]);
	ASSERT_FALSE(inducesAStar(tmp2, g));

	g.addEdge(V[1], V[2]);
	ASSERT_FALSE(inducesAStar(tmp, g));
	ASSERT_TRUE(inducesAStar({V[0], V[1], V[3]}, g));

	for (auto v : V)
		if (v != V[2])
			g.addEdge(v, V[2]);

	ASSERT_FALSE(inducesAStar(V, g));
	ASSERT_FALSE(inducesAStar(tmp, g));
	ASSERT_FALSE(inducesAStar({V[0], V[2], V[3]}, g));
}

TEST(StarStructure, basicOperations)
{
	Graph g(10);
	auto V = g.getVertices();

	Star s;
	ASSERT_EQ(s.size(), 0);
	ASSERT_FALSE(s.isDefinedCenter());
	ASSERT_EQ(s.getLeaves().size(), 0);
	ASSERT_EQ(s.getAllVertices().size(), 0);

	Star s1(V[0], {});
	ASSERT_EQ(s1.size(), 1);
	ASSERT_TRUE(s1.isDefinedCenter());
	ASSERT_EQ(s1.getLeaves().size(), 0);
	ASSERT_EQ(s1.getAllVertices()[0], V[0]);

	Star s2(V[1], {V[2]});
	ASSERT_EQ(s2.size(), 2);
	ASSERT_TRUE(s2.isDefinedCenter());
	ASSERT_EQ(s2.getLeaves(), set<vertex>{V[2]});
	{
		auto alls = s2.getAllVertices();
		ASSERT_EQ(set<vertex>(alls.begin(), alls.end()), set<vertex>({V[1], V[2]}));
	}

	Star s3(V[0], {V[1], V[2], V[3]});
	ASSERT_EQ(s3.size(), 4);
	ASSERT_TRUE(s3.isDefinedCenter());
	ASSERT_EQ(s3.getLeaves(), set<vertex>({V[1], V[2], V[3]}));
	{
		auto alls = s3.getAllVertices();
		ASSERT_EQ(set<vertex>(alls.begin(), alls.end()), set<vertex>({V[1], V[2], V[3], V[0]}));
	}

	Star s4({V[1], V[2], V[3]});
	ASSERT_FALSE(s4.isDefinedCenter());
}

TEST(StarStructure, modifyingStar)
{
	Graph g(10);
	auto V = g.getVertices();
	Star s;
	ASSERT_FALSE(s.isDefinedCenter());
	s.setCenter(V[0]);
	ASSERT_TRUE(s.isDefinedCenter());

	s.unsetCenter();
	ASSERT_FALSE(s.isDefinedCenter());

	s.setCenter(V[1]);
	s.addLeaf(V[0]);
	ASSERT_EQ(s.size(), 2);
	s.removeLeaf(V[0]);
	ASSERT_EQ(s.getAllVertices().size(), 1);
}

TEST(StarPartitions, definedStar)
{
	Graph g(7);
	auto V = g.getVertices();
	set<vertex> l;
	for (auto v : V)
		if (v != V[2])
			g.addEdge(v, V[2]), l.insert(v);

	ASSERT_TRUE(inducesAStar(V, g));
	ASSERT_TRUE(inducesAStar(Star(V[2], l), g));

	ASSERT_TRUE(inducesAStar(Star(V[2], {V[1], V[3], V[0]}), g));

	ASSERT_TRUE(inducesAStar(Star(V[2], {}), g));
	ASSERT_FALSE(inducesAStar(Star(set<vertex>{}), g));
	ASSERT_FALSE(inducesAStar(Star(), g));

	ASSERT_FALSE(inducesAStar(Star({V[3]}), g));

	ASSERT_FALSE(inducesAStar(Star(V[1], {V[3]}), g));
	ASSERT_TRUE(inducesAStar(Star(V[1], {V[2]}), g));
	ASSERT_TRUE(inducesAStar(Star(V[5], {V[2]}), g));

	ASSERT_FALSE(inducesAStar(Star(V[4], {V[1]}), g));
}

TEST(parititoning, simple1)
{
	Graph g(4);
	auto V = g.getVertices();
	vector<vector<vertex>> P;
	for (auto v : V)
		P.push_back({v});

	ASSERT_TRUE(isPartitioning(V, P));

	ASSERT_TRUE(isPartitioning(V, {{}, {}, {}, V}));
	ASSERT_TRUE(isPartitioning(V, {V, {}}));
	ASSERT_TRUE(isPartitioning(V, {V}));

	ASSERT_TRUE(isPartitioning(V, {{V[1], V[2], V[3]}, {V[0]}}));

	ASSERT_TRUE(isPartitioning(V, {{V[0]}, {}, {V[1], V[2], V[3]}}));

	ASSERT_TRUE(isPartitioning({V[1]}, {{V[1]}}));
	ASSERT_TRUE(isPartitioning({}, {{}, {}}));
}

TEST(parititoning, notPartitioning)
{
	Graph g(5);
	auto V = g.getVertices();

	ASSERT_TRUE(isPartitioning(V, {{}, {}, {}, V, {}, {}, {}, {}, {}}));

	ASSERT_FALSE(isPartitioning(V, {{}, {}, {}, {}, {}, {}, {}, {}, {}}));
	ASSERT_FALSE(isPartitioning(V, {}));

	ASSERT_FALSE(isPartitioning({V[0], V[1]}, {{V[0]}, {V[1]}, {V[0]}}));
	ASSERT_FALSE(isPartitioning({V[0], V[1]}, {{V[0]}, {}, {V[1], V[0]}}));
}

TEST(typeOfStar, independent)
{
	Graph g(7);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);

	ASSERT_EQ(getType(Star({V[0]}), g), IS);
	ASSERT_EQ(getType(Star({V[5]}), g), IS);

	ASSERT_EQ(getType(Star({V[1]}), g), IS);

	ASSERT_EQ(getType(Star({V[1], V[3]}), g), IS);
	ASSERT_EQ(getType(Star({V[1], V[4]}), g), IS);
	ASSERT_EQ(getType(Star({V[1], V[4], V[5]}), g), IS);
	ASSERT_EQ(getType(Star({V[0], V[4], V[5]}), g), IS);

	g.addEdge(V[1], V[3]);

	ASSERT_EQ(getType(Star({V[0], V[3]}), g), IS);
}

TEST(typeOfStar, star)
{
	Graph g(7);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	g.addEdge(V[2], V[1]);
	g.addEdge(V[3], V[1]);

	ASSERT_EQ(getType(Star(V[1], {V[2], V[0], V[3]}), g), STAR);
	ASSERT_EQ(getType(Star(V[1], {V[2]}), g), STAR);
	ASSERT_EQ(getType(Star(V[1], {V[3]}), g), STAR);
	ASSERT_EQ(getType(Star(V[3], {V[1]}), g), STAR);
	ASSERT_EQ(getType(Star(V[3], {}), g), STAR);

	ASSERT_EQ(getType(Star(V[5], {}), g), STAR);
}

TEST(typeOfStar, invalid)
{
	Graph g(7);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	g.addEdge(V[0], V[2]);
	g.addEdge(V[0], V[3]);
	g.addEdge(V[0], V[4]);
	g.addEdge(V[0], V[5]);

	ASSERT_EQ(getType(Star(V[0], {V[1], V[2], V[3]}), g), STAR);
	ASSERT_EQ(getType(Star(V[0], {V[5], V[4], V[3]}), g), STAR);

	g.addEdge(V[1], V[2]);
	ASSERT_EQ(getType(Star(V[0], {V[5], V[4], V[3]}), g), STAR);

	// no edge between center and leaf
	ASSERT_EQ(getType(Star(V[3], {V[4]}), g), INVALID);
	ASSERT_EQ(getType(Star(V[5], {V[6]}), g), INVALID);
	ASSERT_EQ(getType(Star(V[6], {V[5], V[4]}), g), INVALID);
	ASSERT_EQ(getType(Star(V[1], {V[2], V[5], V[4]}), g), INVALID);
	ASSERT_EQ(getType(Star(V[1], {V[2], V[4]}), g), INVALID);

	// leafs dont create IS
	ASSERT_EQ(getType(Star({V[1], V[2]}), g), INVALID);
	ASSERT_EQ(getType(Star({V[5], V[1], V[2]}), g), INVALID);
	ASSERT_EQ(getType(Star({V[0], V[1], V[2]}), g), INVALID);

	ASSERT_EQ(getType(Star(V[0], {V[3], V[2], V[1]}), g), INVALID);
	ASSERT_EQ(getType(Star(V[0], {V[2], V[1]}), g), INVALID);
	ASSERT_EQ(getType(Star(V[1], {V[2], V[0]}), g), INVALID);
}

TEST(addingVertex, asLeaf)
{
	Graph g(10);

	auto V = g.getVertices();
	Star S0 = Star(V[0], {});

	g.addEdge(V[1], V[2]);
	Star S1 = Star(V[1], {V[2]});

	g.addEdge(V[3], V[4]);
	g.addEdge(V[3], V[5]);
	g.addEdge(V[3], V[6]);
	Star S3 = Star(V[3], {V[4], V[5], V[6]});

	Star independent = Star({V[7], V[8], V[9]});

	vertex newVertex = g.addVertex();

	ASSERT_FALSE(canAddVertex(g, newVertex, S0, false));
	g.addEdge(V[0], newVertex);
	ASSERT_TRUE(canAddVertex(g, newVertex, S0, false));

	ASSERT_FALSE(canAddVertex(g, newVertex, S1, false));
	g.addEdge(V[1], newVertex);
	ASSERT_TRUE(canAddVertex(g, newVertex, S1, false));

	g.removeEdge(V[1], newVertex);
	g.addEdge(V[2], newVertex);
	ASSERT_FALSE(canAddVertex(g, newVertex, S1, false));

	g.addEdge(V[3], newVertex);
	ASSERT_TRUE(canAddVertex(g, newVertex, S3, false));

	ASSERT_TRUE(canAddVertex(g, g.addVertex(), independent, false));
	ASSERT_TRUE(canAddVertex(g, newVertex, independent, false));

	ASSERT_TRUE(canAddVertex(g, newVertex, Star(), false));

	g.addEdge(newVertex, V[8]);
	ASSERT_FALSE(canAddVertex(g, newVertex, independent, false));
}

TEST(addingVertex, asCenter)
{
	Graph g(10);

	auto V = g.getVertices();
	Star S0 = Star(V[0], {});

	g.addEdge(V[1], V[2]);
	Star S1 = Star(V[1], {V[2]});

	g.addEdge(V[3], V[4]);
	g.addEdge(V[3], V[5]);
	g.addEdge(V[3], V[6]);
	Star S3 = Star(V[3], {V[4], V[5], V[6]});

	Star independent = Star({V[7], V[8], V[9]});

	vertex newVertex = g.addVertex();

	ASSERT_FALSE(canAddVertex(g, newVertex, Star({V[0]}), true));
	g.addEdge(V[0], newVertex);
	ASSERT_TRUE(canAddVertex(g, newVertex, Star({V[0]}), true));

	ASSERT_FALSE(canAddVertex(g, newVertex, independent, true));
	for (auto l : independent.getLeaves())
		g.addEdge(newVertex, l);
	ASSERT_TRUE(canAddVertex(g, newVertex, independent, true));

	g.removeEdge(newVertex, V[7]);
	ASSERT_FALSE(canAddVertex(g, newVertex, independent, true));
}

TEST(connectedComponents, oneComponent)
{
	Graph g(5);
	auto V = g.getVertices();
	for(int i = 0; i < 4; i++)
		g.addEdge(V[i], V[i+1]);
	auto cc = getConnectedComponents(g);
	EXPECT_EQ(cc.size(), 1);
	EXPECT_TRUE(isPartitioning(V, cc));
	EXPECT_EQ(set<vertex>(cc[0].begin(), cc[0].end()), set<vertex>(V.begin(), V.end()));
}

TEST(connectedComponents, isolatedVertices)
{
	Graph g(10);
	auto V = g.getVertices();

	//[0-4]
	for(int i = 0; i < 4; i++)
		g.addEdge(V[i], V[i+1]);

	//[5] [6], [9]
	
	//[7-8]
	g.addEdge(V[7], V[8]);
	auto cc = getConnectedComponents(g);

	EXPECT_EQ(cc.size(), 5) << endl;
	EXPECT_TRUE(isPartitioning(V, cc));
}

TEST(diameter, smallG)
{
	Graph g(1);
	EXPECT_EQ(getDiameter(g), 0);
}

TEST(diameter, smallG2)
{
	Graph g(2);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	EXPECT_EQ(getDiameter(g), 1);
}

TEST(diameter, smallG3)
{
	Graph g(3);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	g.addEdge(V[2], V[1]);
	EXPECT_EQ(getDiameter(g), 2);
}

TEST(diameter, smallG4)
{
	Graph g(3);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	g.addEdge(V[2], V[1]);
	g.addEdge(V[2], V[0]);

	EXPECT_EQ(getDiameter(g), 1);
}

TEST(diameter, tree)
{
	Graph g(7);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	g.addEdge(V[2], V[1]);
	g.addEdge(V[1], V[3]);

	g.addEdge(V[3], V[4]);
	g.addEdge(V[2], V[5]);
	g.addEdge(V[4], V[6]);

	EXPECT_EQ(getDiameter(g), 5);
}

TEST(diameter, generalG)
{
	Graph g(8);
	auto V = g.getVertices();
	g.addEdge(V[0], V[1]);
	g.addEdge(V[2], V[1]);
	g.addEdge(V[2], V[3]);
	g.addEdge(V[0], V[3]);

	g.addEdge(V[0], V[4]);
	g.addEdge(V[1], V[5]);
	g.addEdge(V[2], V[6]);
	g.addEdge(V[3], V[7]);

	EXPECT_EQ(getDiameter(g), 4);
}