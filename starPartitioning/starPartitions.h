#pragma once
#include <set>
#include <vector>

enum PartialSolutionType
{
	IS,	  // the set is an independent set and accetps 1 vertex as the center
	STAR, // the set induces a star K_{1, r} and accepts new leaves only
	INVALID
};

class Star
{
	bool definedCenter;
	vertex center = 0;
	std::set<vertex> leaves;

public:
	Star(vertex c, const std::set<vertex> &l) : definedCenter(true), center(c), leaves(l) {}
	Star(const std::set<vertex> &l) : definedCenter(false), leaves(l) {}
	Star() : definedCenter(false) {}

	std::vector<vertex> getAllVertices() const
	{
		std::vector<vertex> ret(leaves.begin(), leaves.end());
		if (isDefinedCenter())
			ret.push_back(center);
		return ret;
	}

	void setCenter(vertex v) { center = v, definedCenter = true; }
	void unsetCenter() { definedCenter = false; }
	bool isDefinedCenter() const { return definedCenter; }
	vertex getCenter() const
	{
		if (!isDefinedCenter())
			throw std::runtime_error("tried to get a center that was not set");
		return center;
	}

	const std::set<vertex> &getLeaves() const { return leaves; }
	void addLeaf(vertex v) { leaves.insert(v); }
	void removeLeaf(vertex v) { leaves.erase(v); }

	size_t size() const { return leaves.size() + isDefinedCenter(); }
};

struct StarPartitions
{
	std::vector<Star> partitions;
	size_t size() const { return partitions.size(); }
};