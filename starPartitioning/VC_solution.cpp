#include "VC_solution.hpp"
#include "bounds.hpp"
#include "genericAlgorithms.hpp"
#include <algorithm>
#include <set>
#include <map>
#include <cassert>
#include "../externalSolvers/dinic.hpp"
#include "validator.hpp"

#ifdef __DEBUG__
#include "../utility/debug.hpp"
#endif

//------------------------------------------------------------------------------------------------------------

int solve_VC(const Graph &g, const std::vector<vertex> &C, StarPartitions &S)
{
	if (g.getVertexcount() == 0)
		return 0;
	if (g.getVertexcount() == 1)
		S = {{Star(g.getVertices()[0], {})}};
	int lb = getLB(g), ub = getUB(g, C);

#ifdef __DEBUG__
	deb(g.getVertexcount());
	deb(lb);
	deb(ub);
#endif

	for (int q = lb; q <= ub; q++)
	{
#ifdef __DEBUG__
		deb(q);
#endif

		S = StarPartitions();
		int solSize;
		if ((solSize = solve_VC(g, C, q, S)) == q)
			return solSize;
	}
	return -1;
}

int solve_VC(const Graph &g, const std::vector<vertex> &C, int q, StarPartitions &S)
{
	std::map<std::pair<vertex, vertex>, int> distanceMatrix;
	auto V = g.getVertices();
	auto D = getDistanceMatrix(g, V);
	int n = V.size();
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			distanceMatrix[{std::min(V[i], V[j]), std::max(V[i], V[j])}] = D[i][j].realValue;

	std::vector<Star> partialStars(q);
	std::vector<PartialSolutionType> types(q, IS);

	return solve_VC(g, C, q, S, partialStars, types, distanceMatrix, 0, 0);
}

int solve_VC(const Graph &g, const std::vector<vertex> &C, int q, StarPartitions &S,
			 std::vector<Star> &partialStars, std::vector<PartialSolutionType> &types,
			 const std::map<std::pair<vertex, vertex>, int> &distanceMatrix,
			 int curVIndex, int firstAvailStar)
{
	if (curVIndex >= (int)C.size())
		return solve_VC(g, C, q, S, partialStars, types, distanceMatrix);

	//check if the rest of unused vertices can still fill up partialStars so that it doesnt contain an emptyset
	int verticesLeft = C.size() - curVIndex, emptySetsLeft = q - firstAvailStar;
	if(verticesLeft < emptySetsLeft)
		return -1;

	vertex curVertex = C[curVIndex];

	for (int i = 0; i < firstAvailStar; i++)
	{
		// add as leaf
		if (canAddVertex(g, curVertex, partialStars[i], false, distanceMatrix))
		{
			// adding leaf does not change type
			partialStars[i].addLeaf(curVertex);

			int size = solve_VC(g, C, q, S, partialStars, types, distanceMatrix, curVIndex + 1, firstAvailStar);
			if (size == q)
				return q;

			partialStars[i].removeLeaf(curVertex);
		}

		// add as center
		if (canAddVertex(g, curVertex, partialStars[i], true, distanceMatrix))
		{
			types[i] = STAR;
			partialStars[i].setCenter(curVertex);

			int size = solve_VC(g, C, q, S, partialStars, types, distanceMatrix, curVIndex + 1, firstAvailStar);
			if (size == q)
				return q;

			partialStars[i].unsetCenter();
			types[i] = IS;
		}
	}

	// curVertex can only be in a new partition wrong answer
	if (firstAvailStar >= q)
		return 0;

	partialStars[firstAvailStar].addLeaf(curVertex);
	types[firstAvailStar] = IS;
	{
		int retSize = solve_VC(g, C, q, S, partialStars, types, distanceMatrix, curVIndex + 1, firstAvailStar + 1);
		if (retSize == q)
			return retSize;
	}
	partialStars[firstAvailStar].removeLeaf(curVertex);

	partialStars[firstAvailStar].setCenter(curVertex);
	types[firstAvailStar] = STAR;
	{
		int retSize = solve_VC(g, C, q, S, partialStars, types, distanceMatrix, curVIndex + 1, firstAvailStar + 1);
		if (retSize == q)
			return retSize;
	}
	types[firstAvailStar] = IS;
	partialStars[firstAvailStar].unsetCenter();

	return 0;
}

int solve_VC(const Graph &g, const std::vector<vertex> &C, int q, StarPartitions &S,
			 const std::vector<Star> &partialStars, const std::vector<PartialSolutionType> &types,
			 const std::map<std::pair<vertex, vertex>, int> &distanceMatrix)
{
	// check if partitioning is valid
	if (!isValid_PartialStarPartitioning(partialStars, g, types))
		return 0;

	// create a bipartite graph B
	REV_BIPARTITE_EDGE edge_interpreter;
	Bipartite B = constructBipartite(g, C, partialStars, types, distanceMatrix, edge_interpreter);

	// get matching on B
	auto [matchingSize, matching] = findMatching(B);
	if (matchingSize < (int)B.A.size() || matchingSize + B.hasSpecial() < (int)B.B.size()) // no solution exists
		return 0;

#ifdef __DEBUG__
	deb(q);
	deb(partialStars.size());
	deb(g.getVertexcount());
	deb(C.size());
	deb(B.A.size());
	deb(B.B.size());
	deb(matchingSize);
#endif

	S = {partialStars};
	for (auto [u, v] : matching)
	{
		auto [g_vertex, i] = edge_interpreter({u, v});

		if (S.partitions[i].isDefinedCenter())
			S.partitions[i].addLeaf(g_vertex);
		else
			S.partitions[i].setCenter(g_vertex);
	}

	return S.size();
}

//------------------------------------------------------------------------------------------------------------

Bipartite constructBipartite(const Graph &g, const std::vector<vertex> &C,
							 const std::vector<Star> &PartialStars, const std::vector<PartialSolutionType> &types,
							 const std::map<std::pair<vertex, vertex>, int> &distanceMatrix,
							 REV_BIPARTITE_EDGE &edge_interpreter)
{
	Bipartite ret;

	// create the partition A representing the vertices not in VC
	std::map<vertex, vertex> A_mapper; // bipartite vertex to vertex in g
	{
		std::set<vertex> VC(C.begin(), C.end()); // faster search in VC
		for (auto u : g.getVertices())
			if (VC.count(u) == 0)
				A_mapper[ret.addNewToA()] = u;
	}

	// creates the partition B representing the partial stars, each F2 type gets its own vertex, all other types are 1 special verex
	std::map<vertex, int> B_Mapper;		   // maps bipartite to P
	std::map<int, vertex> B_MapperInverse; // maps P to bipartite
	{
		ret.setSpecial(ret.addNewToB()); // 1 extra vertex for other types that need only leaves (not F2)

		for (size_t i = 0; i < PartialStars.size(); i++)
		{
			if (types[i] == IS)
			{
				vertex newVertex = ret.addNewToB();

				B_Mapper[newVertex] = i;
				B_MapperInverse[i] = newVertex;
			}
		}
	}

	// create edges of the bipartite graph
	std::map<vertex, int> specialAllowedSet; // maps a_i from bipartite to a partition of type STAR
	{
		for (auto [a_i, gVertex] : A_mapper)
		{
			bool hasPlaceAsLeaf = false;
			for (size_t i = 0; i < PartialStars.size(); i++)
			{
				// a_i can be a center surrounded by IS or leaf of a star
				if (types[i] == IS && canAddVertex(g, gVertex, PartialStars[i], types[i], distanceMatrix))
				{
					ret.addEdge(a_i, B_MapperInverse[i]);
				}
				else if (types[i] == STAR && !hasPlaceAsLeaf && canAddVertex(g, gVertex, PartialStars[i], types[i], distanceMatrix))
				{
					specialAllowedSet[a_i] = i;
					ret.addEdge(a_i, ret.special);
					hasPlaceAsLeaf = true;
				}
			}
		}
	}

	// setup functions to be able to reinterpret vertices of the bipartite graph
	edge_interpreter = [=](std::pair<vertex, vertex> p) -> std::pair<vertex, int>
	{
		if (A_mapper.find(p.first) == A_mapper.end())
			p = {p.second, p.first};
		if (A_mapper.find(p.first) == A_mapper.end())
			throw std::invalid_argument("edge does not exist");

		return {A_mapper.at(p.first),
				p.second == ret.special ? specialAllowedSet.at(p.first) : B_Mapper.at(p.second)};
	};

	return ret;
}

//------------------------------------------------------------------------------------------------------------

std::pair<int, std::map<vertex, vertex>> findMatching(const Bipartite &B)
{
	std::map<vertex, int> m; // reverse maps vertex to the i-th number
	auto V = B.g.getVertices();
	for (int i = 0; i < (int)V.size(); i++)
		m[V[i]] = i;

	Dinic d(V.size() + 2); // extra vertices for source and target
	int s = V.size(), t = V.size() + 1;

	for (auto a_i : B.A) //(s, a)
		d.addEdge(s, m[a_i], 1);

	for (auto a_i : B.A) //(a, b)
		for (auto b_i : B.g.getNeighbours(a_i))
			d.addEdge(m[a_i], m[b_i], 1);

	for (auto b_i : B.B) //(b, t)
	{
		ll capacity = 1;
		if (B.hasSpecial() && b_i == B.special)
			capacity = std::max(0, (int)B.A.size() - (int)B.B.size() + 1); // A - (B-1), the difference between the two partitions and -1 for the special vertex
		d.addEdge(m[b_i], t, capacity);
	}

	auto matchingSize = d.calc(s, t);

	std::map<vertex, vertex> matching;
	for (int i = 0; i < (int)V.size(); i++) // all vertices from network except for s and t
	{
		for (auto e : d.adj[i])
		{
			if (e.to == s || e.to == t)
				continue;
			if (e.flow() != 0 && e.flow() == e.oc)
			{
				matching[V[i]] = V[e.to];
			}
		}
	}

	return {matchingSize, matching};

	// for (auto v : V)
	// {
	// 	for (auto o : g.getNeighbours(v))
	// 		edges[m[v]].push_back(m[o]);
	// }

	// auto [size, matching] = matching_hungarian(edges);

	// std::vector<vertex> ret_matching(g.getVertexcount());
	// for (int i = 0; i < matching.size(); i++)
	// {
	// 	ret_matching[i] = V[matching[i]];
	// }

	// return {size, ret_matching};
}