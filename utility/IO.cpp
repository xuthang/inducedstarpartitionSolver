#include "IO.hpp"
#include "../starPartitioning/starPartitions.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void printUsage(std::ostream &out)
{
	out << "usage: ./a.out graphInputLocation [optional file for treewidth or vertex cover]" << std::endl;
}

configuration loadConfig(int argc, char **argv)
{
	configuration ret;
	if (argc < 2)
	{
		std::cerr << "missing graph file" << std::endl;
		printUsage(std::cerr);
		exit(2);
	}
	ret.graphLocation = std::string(argv[1]);

	if (argc >= 3)
		ret.VCLocation = std::string(argv[2]);

	return ret;
}

Graph loadGraph(const configuration &config)
{
	std::ifstream is(config.graphLocation);
	if (!is)
		throw std::runtime_error("input file does not exist");

	auto get_uc_line = [](std::ifstream &is)
	{
		std::stringstream ss;
		std::string line;
		do
		{
			getline(is, line);
		} while (line[0] == 'c');
		return std::stringstream(move(line));
	};

	std::string p, format;
	size_t n, m;

	auto problemDefinitionLine = get_uc_line(is);
	if (!(problemDefinitionLine >> p >> format >> n >> m) || p != "p")
		throw std::runtime_error("Invalid input format of graph");

	Graph ret(n);
	std::vector<vertex> V = ret.getVertices();
	for (size_t i = 0; i < m; i++)
	{
		auto line = get_uc_line(is);
		vertex u, v;
		if (!(line >> u >> v))
			throw std::runtime_error("Invalid input format  of graph");
		u--, v--;
		ret.addEdge(V[u], V[v]);
	}

	return ret;
}

std::ostream &printGraph(std::ostream &out, const Graph &g)
{
	out << "p edge " << g.getVertexcount() << " " << g.getEdgecount() << "\n";
	for (auto [u, v] : g.getEdges())
		out << "e " << u + 1 << " " << v + 1 << '\n';

	out.flush();
	return out;
}

std::ostream &printSolution(std::ostream &out, const Graph &g, const StarPartitions &S)
{
	out << S.size() << "\n";
	for (const auto &S_i : S.partitions)
	{
		out << S_i.size() << "\n";
		out << S_i.getCenter() << " ";
		for (auto v : S_i.getLeaves())
			out << v << " ";
		out << std::endl;
	}
	return out;
}

std::vector<vertex> loadVC(const configuration &config)
{
	std::ifstream is(config.VCLocation);

	if (!is)
		throw std::runtime_error("vertex cover file cannot be opened");

	auto get_uc_line = [](std::ifstream &is)
	{
		std::stringstream ss;
		std::string line;
		do
		{
			getline(is, line);
		} while (line[0] == 'c');
		return std::stringstream(move(line));
	};

	std::string s, vc;
	size_t n, vcSize;
	auto firstline = get_uc_line(is);

	if (!(firstline >> s >> vc >> n >> vcSize) || s != "s")
		throw std::runtime_error("invalid vertex cover file format");

	std::vector<vertex> ret;

	for (size_t i = 0; i < vcSize; ++i)
	{
		auto line = get_uc_line(is);
		vertex u;
		if (!(line >> u))
			throw std::runtime_error("invalid vertex cover file format");

		u--;
		ret.push_back(u);
	}

	return ret;
}
