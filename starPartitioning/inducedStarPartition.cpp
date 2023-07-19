#include "inducedStarPartition.hpp"
#include "genericAlgorithms.hpp"
#include "VC_Solution.hpp"
#include "../utility/debug.hpp"

int getTrivialSolution(const Graph & g, StarPartitions &S)
{
	S = StarPartitions();
	for(auto u : g.getVertices())
		S.partitions.push_back(Star(u,{}));
	return S.size();
}

int inducedStarPartitioning_vc(const Graph &g, const std::vector<vertex> &vc, StarPartitions &S)
{
	auto cc = getConnectedComponents(g);
	for(auto c : cc)
	{
		Graph sub = g.getInducedSubgraph(c);
		std::vector<vertex> subVC;
		for(auto v : vc)
			if(sub.hasVertex(v)) subVC.push_back(v);
		
		StarPartitions subPartition;
		solve_VC(sub, subVC, subPartition);
		S.partitions.insert(S.partitions.end(), subPartition.partitions.begin(), subPartition.partitions.end());
	}

	return S.size();
}
