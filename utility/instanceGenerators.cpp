
#include "instanceGenerators.hpp"
#include <random>
#include <algorithm>
#include <set>

#include "../utility/debug.hpp"

Graph generateGraph_VC(size_t vertexCount, size_t VC_size, double edgeRate)
{
	return generateGraph_VC(vertexCount, VC_size, edgeRate, 727);
}

Graph generateGraph_VC(size_t vertexCount, size_t VC_size, double edgeRate, int seed)
{
	std::vector<vertex> throwaway;
	return generateGraph_VC(vertexCount, VC_size, edgeRate, seed, throwaway);
}

Graph generateGraph_VC(size_t vertexCount, size_t VC_size, double edgeRate, int seed, std::vector<vertex> & VC)
{
	Graph ret(vertexCount);

	// init randomness
	std::mt19937 rng(seed);

	auto V = ret.getVertices();

	// select VC_size random vertices from V
	std::sample(V.begin(), V.end(), std::back_inserter(VC), VC_size, rng);

	std::vector<vertex> IS; // vertices outside of VC
	std::set<vertex> VCFinder(VC.begin(), VC.end());
	std::copy_if(V.begin(), V.end(), std::back_inserter(IS), [&](vertex v)
				 { return VCFinder.count(v) == 0; });

	auto addEdges = [&](const std::vector<std::pair<vertex, vertex>> &allEdges, int cnt)
	{
		std::vector<std::pair<vertex, vertex>> edges;
		std::sample(allEdges.begin(), allEdges.end(), std::back_inserter(edges), cnt, rng);
		for (auto [u, v] : edges)
			ret.addEdge(u, v);
	};

	// generate edges of VC with IS
	std::vector<std::pair<vertex, vertex>> bipartite;
	for (auto v : VC)
		for (auto u : IS)
			bipartite.push_back({v, u});
	addEdges(bipartite, bipartite.size()* edgeRate);


	// generate edges within VC
	std::vector<std::pair<vertex, vertex>> K_n;
	for(int i = 0; i < (int)VC.size(); ++i)
		for(int j = i+1; j < (int)VC.size(); ++j)
			K_n.push_back({VC[i], VC[j]});
	addEdges(K_n, K_n.size()* edgeRate);

	// remove isolated vertices
	for(auto v : IS)
	{
		if(ret.getDegree(v) > 0) continue;
		std::vector<vertex> neigh;
		std::sample(VC.begin(), VC.end(), std::back_inserter(neigh), 1, rng);

		ret.addEdge(v, neigh[0]);
	}

	//remove isolated vertex from VC
	std::set<vertex> nonIsolatedVC;
	for(auto v: VC)
		if(ret.getDegree(v) > 0) nonIsolatedVC.insert(v);
	
	for(auto v: VC)
	{
		if(ret.getDegree(v) > 0) continue;
		std::vector<vertex> neigh;
		std::sample(nonIsolatedVC.begin(), nonIsolatedVC.end(), std::back_inserter(neigh), 1, rng);
		ret.addEdge(v, neigh[0]);
		nonIsolatedVC.insert(v);
	}

	return ret;
}