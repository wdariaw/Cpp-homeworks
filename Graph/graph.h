#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_set>


class Graph {
public:
	typedef size_t vertex_t;

	virtual size_t GetNumVertices() const = 0;
	virtual size_t GetNumEdges() const = 0;

	virtual bool HasEdge(vertex_t from, vertex_t to) const = 0;
	virtual void AddEdge(vertex_t from, vertex_t to) = 0;
	virtual void DeleteEdge(vertex_t from, vertex_t to) = 0;

	virtual const std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const = 0;

	virtual void Transpose() = 0;
};

struct StructGraphAdjMatrix {
	std::vector<std::vector<bool>> graph_matrix_;
	size_t num_of_edges_;
};

class GraphAdjMatrix : public Graph {
	StructGraphAdjMatrix graph_;
public:
	GraphAdjMatrix(vertex_t num_of_vertices);

	size_t GetNumVertices() const;

	size_t GetNumEdges() const;

	bool HasEdge(vertex_t from, vertex_t to) const;

	void AddEdge(vertex_t from, vertex_t to);

	void DeleteEdge(vertex_t from, vertex_t to);

	const std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const;

	void Transpose();
};


struct StructGraphAdjLists {
	std::vector<std::unordered_set<int>> graph_vect_;
	size_t num_of_edges_;
};

class GraphAdjLists : public Graph {
	StructGraphAdjLists graph_;
public:
	
	GraphAdjLists(vertex_t num_of_vertices);

	size_t GetNumVertices() const;

	size_t GetNumEdges() const;
	bool HasEdge(vertex_t from, vertex_t to) const;

	void AddEdge(vertex_t from, vertex_t to);

	void DeleteEdge(vertex_t from, vertex_t to);

	const std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const;

	void Transpose();

};

#endif