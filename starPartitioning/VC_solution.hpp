#pragma once

#include "../graph.hpp"
#include "starPartitions.h"
#include <vector>
#include <utility>
#include <functional>
#include <string>

struct Bipartite
{
	Graph g;
	std::set<vertex> A, B;
	bool specialIsSet = false;
	vertex special; // special vertex in B that represents all vertices of type F1, F3 and F4

	vertex addNewToA()
	{
		vertex newVertex = g.addVertex();
		A.insert(newVertex);
		return newVertex;
	}

	vertex addNewToB()
	{
		vertex newVertex = g.addVertex();
		B.insert(newVertex);
		return newVertex;
	}

	vertex setSpecial(vertex v)
	{
		specialIsSet = true;
		return special = v;
	}

	bool hasSpecial() const { return specialIsSet; }

	void addEdge(vertex u, vertex v)
	{
		if (A.count(u) == 0)
			std::swap(u, v);

		if (A.count(u) == 0 || B.count(v) == 0)
			throw std::invalid_argument("vertices " + std::to_string(u) + " and " + std::to_string(v) + " dont exists, |A| = " + std::to_string(A.size()) + " and |B| = " + std::to_string(B.size()));
		g.addEdge(u, v);
	}
};

/**
 * @param g a connected graph
 * @param C the vertex cover of graph G
 * @param S the resulting partitioning
 * @return int min number of stars needed
 */
int solve_VC(const Graph &g, const std::vector<vertex> &C, StarPartitions &S);

int solve_VC(const Graph &g, const std::vector<vertex> &C, int q, StarPartitions &S);

int solve_VC(const Graph &g, const std::vector<vertex> &C, int q, StarPartitions &S,
			 const std::vector<Star> &partialStars, const std::vector<PartialSolutionType> &types,
			 const std::map<std::pair<vertex, vertex>, int> &distanceMatrix);

int solve_VC(const Graph &g, const std::vector<vertex> &C, int q, StarPartitions &S,
			 std::vector<Star> &partialStars, std::vector<PartialSolutionType> &types,
			 const std::map<std::pair<vertex, vertex>, int> &distanceMatrix,
			 int curVIndex, int firstAvailStar);

//--------------------------------------------------------------------------------------------------

using REV_BIPARTITE_EDGE = std::function<std::pair<vertex, int>(const std::pair<vertex, vertex> &)>;
/**
 * @brief creates a bipartite graph that that holds the relation where vertex v in (V - C) can be in the same set with PartialStars[i]
 *
 * @param C the vertex cover of graph g
 * @param PartialStars a partition of C
 * @param ret bipartite graph that we want to construct
 * @param edge_interpreter - maps and edge (a_i, b_i) both from bipartite and returns (u, i) where u in (V - C) and i is index of partition P_i that it should go into
 */
Bipartite constructBipartite(const Graph &g, const std::vector<vertex> &C,
							 const std::vector<Star> &PartialStars, const std::vector<PartialSolutionType> &types,
							 const std::map<std::pair<vertex, vertex>, int> &distanceMatrix,
							 REV_BIPARTITE_EDGE &edge_interpreter);

/**
 * returns a maximum matching in a bipartite graph
 * @param g
 * @return (max matching size, match) where A[i] is matched with B[match[i]], if A[i] is not matched with anyone then it is not present in match
 */
std::pair<int, std::map<vertex, vertex>> findMatching(const Bipartite &g);
