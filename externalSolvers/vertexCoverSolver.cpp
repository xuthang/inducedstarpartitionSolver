#include "vertexCoverSolver.hpp"
#include <map>

#ifdef __GUROBI__
#include "gurobi_c++.h"
std::vector<vertex> getVertexCover(const Graph &g)
{
	GRBEnv env = GRBEnv(true);
	env.set(GRB_StringParam_LogFile, "logs/vc.log" );
	env.set(GRB_IntParam_OutputFlag, 0);
	env.start();
	GRBModel model = GRBModel(env);
	
	env.start();
	std::map<vertex, GRBVar> vars;
	GRBLinExpr obj;

	for (auto u : g.getVertices())
	{
		obj += (vars[u] = model.addVar(0, 1, 1, GRB_BINARY));
	}

	for (auto u : g.getVertices())
	{
		for (auto v : g.getNeighbours(u))
		{
			if (u < v)
				model.addConstr(vars[u] + vars[v] >= 1); // either u or v in solution
		}
	}

	model.optimize();

	std::vector<vertex> ret;
	for (auto u : g.getVertices())
		if (vars[u].get(GRB_DoubleAttr_X) >= 1)
			ret.push_back(u);

	return ret;
}
#else
std::vector<vertex> getVertexCover(const Graph &g)
{
	std::vector<vertex> ret;
	throw std::runtime_error("vertex cover algorithm is not implemented, please provide vertex cover on the input");
	return ret;	
}
#endif