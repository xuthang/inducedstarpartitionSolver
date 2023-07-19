#include "genericAlgorithms.hpp"
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <limits>
#include "../utility/debug.hpp"

void bfs(const Graph &g, vertex s, std::map<vertex, bool> &visited, std::vector<vertex> &component)
{
	std::queue<vertex> q;
	q.push(s);
	visited[s] = true;
	component.push_back(s);
	while (!q.empty())
	{
		auto cur = q.front();
		q.pop();
		for (auto u : g.getNeighbours(cur))
		{
			if (!visited[u])
			{
				q.push(u);
				visited[u] = true;
				component.push_back(u);
			}
		}
	}
}

std::vector<std::vector<vertex>> getConnectedComponents(const Graph &g)
{
	std::vector<std::vector<vertex>> ret;
	std::map<vertex, bool> visited;
	for (auto v : g.getVertices())
		visited[v] = false;

	for (auto v : g.getVertices())
	{
		if (visited[v])
			continue;

		std::vector<vertex> component;
		bfs(g, v, visited, component);
		ret.push_back(component);
	}
	return ret;
}

std::vector<std::vector<NUM>> getDistanceMatrix(const Graph &g, const std::vector<vertex> &V)
{
	int n = V.size();
	std::map<vertex, int> mapper;
	for (int i = 0; i < n; ++i)
		mapper[V[i]] = i;

	// first if exists path, second the len
	std::vector<std::vector<NUM>> D(n, std::vector<NUM>(n));
	for (auto v : V)
	{
		D[mapper[v]][mapper[v]] = NUM(0);
		for (auto o : g.getNeighbours(v))
			D[mapper[v]][mapper[o]] = NUM(1);
	}

	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				D[i][j] = std::min(D[i][j], D[i][k] + D[k][j]);
	return D;
}

int getDiameter(const Graph &g)
{
	int n = g.getVertexcount();
	auto D = getDistanceMatrix(g, g.getVertices());

	int maxDistance = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (!D[i][j].isInf)
				maxDistance = std::max(maxDistance, D[i][j].realValue);

	return maxDistance;
}

bool isIndependentSet(const std::vector<vertex> &S, const Graph &g)
{
	for (size_t i = 0; i < S.size(); i++)
	{
		for (size_t j = i + 1; j < S.size(); j++)
		{
			if (g.hasEdge(S[i], S[j]))
				return false;
		}
	}
	return true;
}

bool isPartitioning(const std::vector<vertex> &total, const std::vector<std::vector<vertex>> &Partitions)
{
	std::map<vertex, bool> visited;
	for (auto v : total)
		visited[v] = false;

	for (const auto &P_i : Partitions)
	{
		for (auto v : P_i)
		{
			auto f = visited.find(v);
			if (f == visited.end())
				throw std::invalid_argument("partition includes a vertex that it should not");

			if (f->second == true) // vertex is in 2 partitions
				return false;

			visited[v] = true;
		}
	}

	for (auto v : total)
		if (!visited[v])
			return false;

	return true;
}

bool canAddVertex(const Graph &g, vertex a, const Star &S_i, bool asCenter)
{
	return canAddVertex(g, a, S_i, asCenter, {});
}

bool canAddVertex(const Graph &g, vertex a, const Star &S_i, bool asCenter,
				  const std::map<std::pair<vertex, vertex>, int> &distanceMatrix)
{
	if (asCenter)
	{
		if (S_i.isDefinedCenter())
			return false;
		auto N_a = g.getNeighbours(a);
		if(!std::includes(N_a.begin(), N_a.end(), S_i.getLeaves().begin(), S_i.getLeaves().end()))
			return false;

		if(!distanceMatrix.empty())
			for(auto o : S_i.getLeaves())
				if(distanceMatrix.at({std::min(a, o), std::max(a, o)}) > 2)
					return false;

		return true;
	}
	else
	{
		// check that a is a leaf of S_i
		if (S_i.isDefinedCenter() && !g.hasEdge(a, S_i.getCenter()))
			return false;

		// check that adding the vertex, the leaves is still an independent set
		for (auto o : S_i.getLeaves())
			if (g.hasEdge(a, o))
				return false;
		
		return true;
	}
}

bool canAddVertex(const Graph &g, vertex a, const Star &S_i, PartialSolutionType starType)
{
	if (starType == INVALID)
		return false;
	return canAddVertex(g, a, S_i, starType == IS);
}

bool canAddVertex(const Graph &g, vertex a, const Star &S_i, PartialSolutionType starType,
				  const std::map<std::pair<vertex, vertex>, int> &distanceMatrix)
{
	if (starType == INVALID)
		return false;
	return canAddVertex(g, a, S_i, starType == IS, distanceMatrix);
}

bool inducesAStar(const Star &S, const Graph &g)
{
	if (!S.isDefinedCenter())
		return false;

	// check that leaves are adjacent to center
	for (auto u : S.getLeaves())
	{
		if (!g.hasEdge(S.getCenter(), u))
			return false;
	}

	if (!isIndependentSet(std::vector<vertex>(S.getLeaves().begin(), S.getLeaves().end()), g))
		return false;

	return true;
}

bool inducesAStar(const std::vector<vertex> &S, const Graph &g)
{
	if (S.size() == 1)
		return true;

	// find vertex with highest degree and set it as center
	for (auto u : S)
	{
		size_t degree = 0;
		for (auto v : S)
		{
			if (u != v)
			{
				if (g.hasEdge(u, v))
					degree++;
				else
					break;
			}
		}

		if (degree == S.size() - 1)
		{
			std::set<vertex> leaves;
			for (auto v : S)
				if (u != v)
					leaves.insert(v);

			return inducesAStar(Star(u, leaves), g);
		}
	}
	return false;
}

PartialSolutionType getType(const Star &S_i, const Graph &g)
{
	if (inducesAStar(S_i, g))
		return STAR;
	else if (isIndependentSet(std::vector<vertex>(S_i.getLeaves().begin(), S_i.getLeaves().end()), g) && !S_i.isDefinedCenter())
		return IS;
	else
		return INVALID;
}

std::vector<PartialSolutionType> getTypes(const std::vector<Star> &PartialStars, const Graph &g)
{
	std::vector<PartialSolutionType> ret;
	for (const auto P_i : PartialStars)
		ret.push_back(getType(P_i, g));
	return ret;
}

bool isValid_PartialStarPartitioning(const std::vector<Star> &PartialStars, const Graph &g, const std::vector<PartialSolutionType> &types)
{
	for (size_t i = 0; i < PartialStars.size(); ++i)
	{
		auto type = getType(PartialStars[i], g);
		if (type == INVALID)
			return false;
		if (types[i] != type)
			throw std::runtime_error("incorrect types calculation");
	}
	return true;
}
