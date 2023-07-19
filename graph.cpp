#include "graph.hpp"
#include <stdexcept>
#include <cassert>
#include <algorithm>

Graph::Graph() : newVertexLabel(0) {}

Graph::Graph(size_t n) : newVertexLabel(0)
{
	for (vertex u = 0; u < n; u++)
		addVertex();
}

//--------------------------------------------------------------

vertex Graph::addVertex(vertex newVertex)
{
	#ifdef __DEBUG__
	if(newVertex < this->newVertexLabel)
		throw std::invalid_argument("tried to add a vertex that might already exist");
	#endif

	newVertexLabel = newVertex;
	
	vertices.insert(newVertexLabel);
	edges[newVertexLabel] = {};
	return newVertexLabel++;
}

vertex Graph::addVertex()
{
	vertices.insert(newVertexLabel);
	edges[newVertexLabel] = {};
	return newVertexLabel++;
}

bool Graph::hasVertex(vertex v) const
{
	return vertices.find(v) != vertices.end();
}

vertex Graph::removeVertex(vertex v)
{
	#ifdef __DEBUG__
	if(!hasVertex(v))
		throw std::invalid_argument("vertex does not exist");
	#endif

	for (auto o : getNeighbours(v))
		removeEdge(o, v);
	vertices.erase(v);
	return v;
}

size_t Graph::getVertexcount() const
{
	return vertices.size();
}

std::vector<vertex> Graph::getVertices() const
{
	return std::vector<vertex>(vertices.begin(), vertices.end());
}

//--------------------------------------------------------------
void Graph::addEdge(vertex u, vertex v)
{
	#ifdef __DEBUG__
	if(!hasVertex(u) || !hasVertex(v))
		throw std::invalid_argument("adding edge where vertex does not exist");
	#endif
		
	if (!hasEdge(u, v))
	{
		edges[u].insert(v);
		edges[v].insert(u);
		m++;
	}
}

void Graph::removeEdge(vertex u, vertex v)
{
	#ifdef __DEBUG__
	if(!hasVertex(u) || !hasVertex(v))
		throw std::invalid_argument("erasing edge where vertex does not exist");
	#endif
		
	if (hasEdge(u, v))
	{
		edges[u].erase(v);
		edges[v].erase(u);
		m--;
	}
}

size_t Graph::getEdgecount() const
{
	return m;
}

std::vector<vertex> Graph::getNeighbours(vertex v) const
{
	#ifdef __DEBUG__
	if(!hasVertex(v))
		throw std::invalid_argument("getting neighbours of vertex that does not exist");
	#endif

	return std::vector<vertex>(edges.at(v).begin(), edges.at(v).end());
}

size_t Graph::getDegree(vertex v) const
{
	#ifdef __DEBUG__
	if(!hasVertex(v))
		throw std::invalid_argument("getting degree of vertex that does not exist");
	#endif

	return edges.at(v).size();
}


bool Graph::hasEdge(vertex u, vertex v) const
{
	#ifdef __DEBUG__
	if(!hasVertex(u) || !hasVertex(v))
		throw std::invalid_argument("query on edge where vertex does not exist");
	#endif

	return edges.at(u).find(v) != edges.at(u).end();
}

std::vector<std::pair<vertex, vertex>> Graph::getEdges() const
{
	std::vector<std::pair<vertex, vertex>> ret;
	for(auto v : getVertices())
	{
		for(auto o : getNeighbours(v))
		{
			if(v < o) ret.push_back({v, o});
		}
	}
	return ret;
}

Graph Graph::getInducedSubgraph(const std::vector<vertex> &vertices) const
{
	std::set<vertex> setV(vertices.begin(), vertices.end());

	#ifdef __DEBUG__
	if(!std::includes(this->vertices.begin(), this->vertices.end(), setV.begin(), setV.end()))
		throw std::invalid_argument("gave a vertex that does not exist in graph");
	#endif

	Graph ret;

	for(auto v : setV)
		ret.addVertex(v);
	
	for(auto v : setV)
		for(auto neigh : this->getNeighbours(v))
			if(v < neigh && setV.find(neigh) != setV.end())
				ret.addEdge(v, neigh);
	return ret;
}
