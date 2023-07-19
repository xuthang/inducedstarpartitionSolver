#include "validator.hpp"
#include "genericAlgorithms.hpp"
#include <vector>

bool isValidStarPartition(const Graph &g, const StarPartitions &S)
{
	return isValidStarPartition(g, S, S.size());
}

bool isValidStarPartition(const Graph &g, const StarPartitions &S, int solutionSize)
{
	if ((int)S.size() > solutionSize)
		return false;

	std::vector<std::vector<vertex>> partitions;
	for (const auto &S_i : S.partitions)
		partitions.push_back(S_i.getAllVertices());

	if (!isPartitioning(g.getVertices(), partitions))
		return false;

	for (const auto &S_i : S.partitions)
		if (!inducesAStar(S_i, g))
			return false;

	return true;
}