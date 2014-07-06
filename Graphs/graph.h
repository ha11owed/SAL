#pragma once

#include <vector>

class Graph
{
public:
	Graph(size_t nodeCount) : g(nodeCount) {}
	virtual ~Graph() {}

	void add(int from, int to, double cost);
	void addUndirected(int from, int to, double cost);

	void maxFlow(Graph& outMaxFlow);

private:
	struct Edge
	{
		int from;
		int to;
		double cost;
	};

	std::vector<std::vector<Edge>> g;
};

// ---- Inline implementation ----
inline void Graph::add(int from, int to, double cost)
{
	Edge edge;
	edge.from = from;
	edge.to = to;
	edge.cost = cost;
	g[from].push_back(edge);
}

inline void Graph::addUndirected(int from, int to, double cost)
{
	add(from, to, cost);
	add(to, from, cost);
}

inline void Graph::maxFlow(Graph& outMaxFlow)
{
	outMaxFlow.g = g;
	std::vector<std::vector<Edge>>& outG = outMaxFlow.g;
	for (size_t i = 0; i < outG.size(); i++)
	{
		for (size_t j = 0; j < outG[i].size(); j++)
		{
			outG[i][j].cost = 0.0;
		}
	}
}
