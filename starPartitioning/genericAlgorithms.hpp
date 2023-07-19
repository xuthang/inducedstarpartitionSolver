#pragma once

#include "../graph.hpp"
#include "starPartitions.h"
#include <vector>
#include <map>

/**
 * @brief gets all vertices that are in the same component as s
 */
void bfs(const Graph &g, vertex s, std::map<vertex, bool> &visited, std::vector<vertex> &component);

/**
 * @brief Get the Connected Components of the graph
 * @return std::vector<std::vector<vertex>> set of components, each component is decribed by its set of vertices
 */
std::vector<std::vector<vertex>> getConnectedComponents(const Graph &g);

struct NUM
{
	bool isInf;
	int realValue;
	NUM operator+(const NUM &o) const { return (isInf || o.isInf) ? NUM() : NUM(realValue + o.realValue); }
	bool operator<(const NUM &o) const
	{
		if (isInf)
			return false;
		else if (o.isInf)
			return true;
		return realValue < o.realValue;
	}
	friend std::ostream &operator<<(std::ostream &out, const NUM &n) { return out << (n.isInf ? "~" : std::to_string(n.realValue)); }
	NUM() : isInf(true), realValue(std::numeric_limits<int>::max()) {}
	NUM(int val) : isInf(false), realValue(val) {}
};


/**
 * calculates the distance matrix for every two vertices using Floyd Warshalls algorithm in n^3
 * the order of vertices is the same as g.getVertices()
 * thus the row ret[0] returns distance of all vertices from g.getVertices()[0]
*/
std::vector<std::vector<NUM>> getDistanceMatrix(const Graph &g, const std::vector<vertex> &V);

/**
 * @brief find the distance of 2 vertices that have maximal shortest path of all pairs of vertices
 * expects the graph g to be connected
 * uses floydwarshall alg in time O(n^3)
 * @return int 0 if g has 1 vertex, a non negative distance otherwise
 */
int getDiameter(const Graph &g);

bool isIndependentSet(const std::vector<vertex> &S, const Graph &g);

/**
 * @brief checks if Partitions is a partitioning of total
 * allows empty sets in partitions
 * can throw an exception of Partitions contains a vertex that is not present in total
 * otherwise returns true or false
 */
bool isPartitioning(const std::vector<vertex> &total, const std::vector<std::vector<vertex>> &Partitions);

bool inducesAStar(const Star &S, const Graph &g);
bool inducesAStar(const std::vector<vertex> &S, const Graph &g);

PartialSolutionType getType(const Star &S_i, const Graph &g);

std::vector<PartialSolutionType> getTypes(const std::vector<Star> &PartialStars, const Graph &g);

bool isValid_PartialStarPartitioning(const std::vector<Star> &PartialStars, const Graph &g, const std::vector<PartialSolutionType> &types);

/**
 * @brief checks if the vertex a can be added into the star S_i either as center or as leaf depending on the last param
 */
bool canAddVertex(const Graph &g, vertex a, const Star &S_i, bool asCenter);

bool canAddVertex(const Graph &g, vertex a, const Star &S_i, bool asCenter,
				  const std::map<std::pair<vertex, vertex>, int> &distanceMatrix);

/**
 * @brief checks if the vertex a can be added into the star S_i either as center or as leaf
 */
bool canAddVertex(const Graph &g, vertex a, const Star &S_i, PartialSolutionType starType);

bool canAddVertex(const Graph &g, vertex a, const Star &S_i, PartialSolutionType starType,
				  const std::map<std::pair<vertex, vertex>, int> &distanceMatrix);
