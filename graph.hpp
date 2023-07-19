#pragma once
#include <vector>
#include <set>
#include <map>
#include <utility>

using vertex = size_t;

class Graph
{
public:
	Graph();
	Graph(size_t n);

	bool hasVertex(vertex v) const;
	bool hasEdge(vertex u, vertex v) const;

	size_t getVertexcount() const;
	size_t getEdgecount() const;
	std::vector<vertex> getVertices() const;
	std::vector<vertex> getNeighbours(vertex v) const;
	size_t getDegree(vertex v) const;
	std::vector<std::pair<vertex, vertex>> getEdges() const;

	vertex addVertex();
	vertex addVertex(vertex newVertex);

	/**
	 * @brief adds undirected edge between u, v. Can be called even if edge already exists, in that case does nothing
	 */
	void addEdge(vertex u, vertex v);

	vertex removeVertex(vertex v);
	void removeEdge(vertex u, vertex v);

	Graph getInducedSubgraph(const std::vector<vertex> &vertices) const;
	//--------------------------------------------------
private:
	vertex newVertexLabel;
	std::set<vertex> vertices;
	size_t m = 0;
	std::map<vertex, std::set<vertex>> edges;
};
