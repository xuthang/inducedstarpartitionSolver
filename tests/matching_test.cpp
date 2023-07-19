#include <bits/stdc++.h>
#include "gtest/gtest.h"
#include "../starPartitioning/VC_solution.hpp"
#include "../externalSolvers/dinic.hpp"

#include <set>
using namespace std;

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

TEST(katclDinic, simple1)
{
	Dinic d(3);
	ll s = 0, t = 2;
	d.addEdge(0, 1, 1);
	d.addEdge(1, 2, 3);
	ll size = d.calc(s, t);

	ASSERT_EQ(size, 1);
}

TEST(katclDinic, simple2)
{
	Dinic d(5);
	ll s = 0, t = 4;
	d.addEdge(s, 1, 1);
	d.addEdge(s, 2, 1);
	d.addEdge(s, 3, 3);

	d.addEdge(1, t, 1);
	d.addEdge(2, t, 1);
	d.addEdge(3, t, 3);

	ll size = d.calc(s, t);

	ASSERT_EQ(size, 5);
}

TEST(katclDinic, simple3)
{
	Dinic d(5);
	ll s = 0, t = 4;
	d.addEdge(s, 1, 1);
	d.addEdge(s, 2, 1);
	d.addEdge(s, 3, 3);

	d.addEdge(1, t, 1);
	d.addEdge(2, t, 1);
	d.addEdge(3, t, 4);

	ll size = d.calc(s, t);

	ASSERT_EQ(size, 5);
}

TEST(katclDinic, edgeInspectionSimple)
{
	Dinic d(2);
	ll s = 0, t = 1;
	d.addEdge(s, t, 1);
	ll size = d.calc(s, t);
	ASSERT_EQ(size, 1);
	ASSERT_EQ(d.adj[s][0].flow(), d.adj[s][0].oc);
}

TEST(katclDinic, edgeInspection)
{
	Dinic d(5);
	ll s = 0, t = 4;
	d.addEdge(s, 1, 1);
	d.addEdge(s, 2, 1);
	d.addEdge(s, 3, 3);

	d.addEdge(1, t, 1);
	d.addEdge(2, t, 1);
	d.addEdge(3, t, 4);

	d.calc(s, t);

	for (auto e : d.adj[s])
		ASSERT_EQ(e.flow(), (e.to == 3 ? 3 : 1));

	for (auto e : d.adj[1])
	{
		if (e.to == s)
			ASSERT_EQ(e.flow(), 0);
		else
			ASSERT_EQ(e.flow(), 1);
	}
}

TEST(matching, K2)
{
	Bipartite bipartite;
	vertex u = bipartite.addNewToA();
	vertex v = bipartite.addNewToB();
	bipartite.addEdge(u, v);

	auto [size, matching] = findMatching(bipartite);
	ASSERT_EQ(size, 1);
	ASSERT_TRUE(matching.size() > 0);
}

TEST(matching, easyMatching)
{
	Bipartite bipartite;
	vector<vertex> A, B;
	for (int i = 0; i < 5; i++)
	{
		A.push_back(bipartite.addNewToA());
		B.push_back(bipartite.addNewToB());
		bipartite.addEdge(A.back(), B.back());
	}

	auto [size, matching] = findMatching(bipartite);
	ASSERT_EQ(size, 5);

	set<vertex> visited;
	for (auto [u, v] : matching)
	{
		ASSERT_EQ(visited.count(u), 0);
		ASSERT_EQ(visited.count(v), 0);

		visited.insert(u);
		visited.insert(v);
		ASSERT_TRUE(bipartite.g.hasEdge(u, v));
	}
	ASSERT_EQ(visited.size(), size*2);
}

TEST(matching, connected)
{
	Bipartite bipartite;
	vector<vertex> A, B;
	for (int i = 0; i < 3; i++)
	{
		A.push_back(bipartite.addNewToA());
		B.push_back(bipartite.addNewToB());
	}

	bipartite.addEdge(A[0], B[0]);
	bipartite.addEdge(A[1], B[0]);
	bipartite.addEdge(A[1], B[1]);
	bipartite.addEdge(A[2], B[0]);
	bipartite.addEdge(A[0], B[2]);

	auto [size, matching] = findMatching(bipartite);
	ASSERT_EQ(size, 3);

	set<vertex> visited;
	for (auto [u, v] : matching)
	{
		ASSERT_EQ(visited.count(u), 0);
		ASSERT_EQ(visited.count(v), 0);

		visited.insert(u);
		visited.insert(v);
		ASSERT_TRUE(bipartite.g.hasEdge(u, v));
	}

	ASSERT_EQ(visited.size(), size*2);
}

TEST(matching, noPerfectMatching)
{
	Bipartite bipartite;
	vector<vertex> A, B;
	for (int i = 0; i < 5; i++)
	{
		A.push_back(bipartite.addNewToA());
		B.push_back(bipartite.addNewToB());
	}

	bipartite.addEdge(A[0], B[0]);
	bipartite.addEdge(A[1], B[0]);
	bipartite.addEdge(A[1], B[1]);
	bipartite.addEdge(A[2], B[0]);
	bipartite.addEdge(A[0], B[2]);

	bipartite.addEdge(A[4], B[4]);

	auto [size, matching] = findMatching(bipartite);
	ASSERT_EQ(size, 4);

	set<vertex> visited;
	for (auto [u, v] : matching)
	{
		ASSERT_EQ(visited.count(u), 0);
		ASSERT_EQ(visited.count(v), 0);

		visited.insert(u);
		visited.insert(v);
		ASSERT_TRUE(bipartite.g.hasEdge(u, v));
	}

	ASSERT_EQ(visited.size(), size*2);
}

TEST(matching, withSpecialVertexSimple)
{
	Bipartite bipartite;
	vector<vertex> A{bipartite.addNewToA()}, B{bipartite.setSpecial(bipartite.addNewToB())};
	bipartite.addEdge(A[0], B[0]);
	auto [size, matching] = findMatching(bipartite);
	ASSERT_EQ(size, 1);
	ASSERT_TRUE(matching.size() > 0);
}

TEST(matching, withSpecialVertex1)
{
	Bipartite bipartite;
	bipartite.setSpecial(bipartite.addNewToB());
	for (int i = 0; i < 5; i++)
	{
		vertex v = bipartite.addNewToA();
		bipartite.addEdge(v, bipartite.special);
	}

	auto [size, matching] = findMatching(bipartite);
	ASSERT_EQ(size, 5);

	set<vertex> visited;
	for (auto [u, v] : matching)
	{
		for(auto x : {u, v})
		{
			if(x != bipartite.special)
			{
				ASSERT_EQ(visited.count(x), 0);
			}

			visited.insert(x);
		}
		ASSERT_TRUE(bipartite.g.hasEdge(u, v));
	}

}

TEST(matching, withSpecialVertex2)
{
	Bipartite bipartite;
	vector<vertex> A, B;
	for (int i = 0; i < 5; i++)
		A.push_back(bipartite.addNewToA());

	for (int i = 0; i < 3; i++)
		B.push_back(bipartite.addNewToB());

	bipartite.setSpecial(B[0]);

	bipartite.addEdge(A[0], B[0]);
	bipartite.addEdge(A[0], B[1]);
	bipartite.addEdge(A[0], B[2]);

	bipartite.addEdge(A[1], B[1]);
	bipartite.addEdge(A[1], B[2]);

	bipartite.addEdge(A[2], B[0]);

	bipartite.addEdge(A[3], B[2]);

	bipartite.addEdge(A[4], B[2]);
	bipartite.addEdge(A[4], B[0]);

	auto [size, matching] = findMatching(bipartite);
	ASSERT_EQ(size, 5);

	set<vertex> visited;
	for (auto [u, v] : matching)
	{
		for(auto x : {u, v})
		{
			if(x != bipartite.special)
			{
				ASSERT_EQ(visited.count(x), 0);
			}

			visited.insert(x);
		}
		ASSERT_TRUE(bipartite.g.hasEdge(u, v));
	}
}

TEST(matching, ABigger)
{
	Bipartite bipartite;
	vector<vertex> A, B;
	for(int i = 0; i < 10; i++)
		A.push_back(bipartite.addNewToA());
	for(int j = 0; j < 4; j++)
		B.push_back(bipartite.addNewToB());
	
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 4; j++)
			bipartite.addEdge(A[i], B[j]);
	auto [size, matching] = findMatching(bipartite);
	EXPECT_EQ(size, 4);
	EXPECT_EQ(matching.size(), 4);

	bipartite.setSpecial(B.back());
	auto [size2, matching2] = findMatching(bipartite);
	EXPECT_EQ(size2, 10);
	EXPECT_EQ(matching2.size(), 10);
}

TEST(matching, BBigger)
{
	Bipartite g;
	vector<vertex> A, B;
	for(int i = 0; i < 5; i++)
		A.push_back(g.addNewToA());
	for (int j = 0; j < 10; j++)
		B.push_back(g.addNewToB());
	
	for(int i = 0; i < 5; i++)
	for (int j = 0; j < 10; j++)
		g.addEdge(A[i], B[j]);

	auto [size, matching] = findMatching(g);
	EXPECT_EQ(size, 5);
	EXPECT_EQ(matching.size(), 5);

	g.setSpecial(B[0]);
	auto [size2, matching2] = findMatching(g);
	EXPECT_EQ(size2, 5);
	EXPECT_EQ(matching2.size(), 5);
	
}

TEST(matching, bigGraph)
{
	Bipartite bipartite;
	vector<vertex> A, B;
	for(int i= 0; i < 5000; i++)
		A.push_back(bipartite.addNewToA());
	
	for(int i= 0; i < 100; i++)
		B.push_back(bipartite.addNewToB());
	
	bipartite.setSpecial(B[0]);

	for(int i= 0; i < 5000; i++)
		for(int j= 0; j < 100; j++)
			bipartite.addEdge(A[i], B[j]);

	srand(1000);

	int removeEdges = rand() % (bipartite.g.getEdgecount() / 4);
	for(int i = 0; i < removeEdges; i++)
	{
		int u = rand()% A.size(), v = rand()% B.size();
		if(bipartite.g.hasEdge(A[u], B[v]))
			bipartite.g.removeEdge(A[u], B[v]);
	}
	
	auto [size, matching] = findMatching(bipartite);
	set<vertex> visited;
	for (auto [u, v] : matching)
	{
		for(auto x : {u, v})
		{
			if(x != bipartite.special)
			{
				ASSERT_EQ(visited.count(x), 0);
			}

			visited.insert(x);
		}
		ASSERT_TRUE(bipartite.g.hasEdge(u, v));
	}

}