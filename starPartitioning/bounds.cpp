#include "bounds.hpp"
#include <functional>
#include <vector>
#include <algorithm>
#include <cmath>
#include "genericAlgorithms.hpp"
#include "../utility/debug.hpp"

int getUB(const Graph &g, const std::vector<vertex> &vc)
{
	return vc.size();
}

int edgeCountLB(const Graph &g)
{
	auto K_ = [](size_t n)
	{ return (n * (n - 1)) / 2; };

	int n = g.getVertexcount(), m = g.getEdgecount();
	int maxE = K_(n);
	for (int q = 1; q <= n / 2; ++q)
	{
		int coveredByStars = n - q;
		int r = n / q;
		int h = n % q;
		int maxCoveredEdges = maxE - h * K_(r + 1) - (q - h) * K_(r) + coveredByStars;
		if (maxCoveredEdges >= m)
			return q;
	}
	return n > 0;
}

int getLB(const Graph &g)
{
	std::vector<int> LBs;
	LBs.push_back(edgeCountLB(g));
	LBs.push_back(std::ceil((getDiameter(g) + 1) / 3.)); //+1 vertices on path, partition that path

	return *std::max_element(LBs.begin(), LBs.end());
}