#include "graph.h"

GraphAdjLists::GraphAdjLists(vertex_t num_of_vertices) {
	graph_.graph_vect_.resize(num_of_vertices);
	graph_.num_of_edges_ = 0;
}

size_t GraphAdjLists::GetNumVertices() const {
	return graph_.graph_vect_.size();
}

size_t GraphAdjLists::GetNumEdges() const {
	return graph_.num_of_edges_;
}

bool GraphAdjLists::HasEdge(vertex_t from, vertex_t to) const {
	if (graph_.graph_vect_[from].find(to) != graph_.graph_vect_[from].end()) {
		return true;
	}
	return false;
}

void GraphAdjLists::AddEdge(vertex_t from, vertex_t to) {
	graph_.graph_vect_[from].insert(to);
	++graph_.num_of_edges_;
}

void GraphAdjLists::DeleteEdge(vertex_t from, vertex_t to) {
	graph_.graph_vect_[from].erase(to);
	--graph_.num_of_edges_;
}

const std::vector<GraphAdjLists::vertex_t> GraphAdjLists::GetAdjacentVertices(vertex_t vertex) const {
	std::vector<vertex_t> adjacent_vertices;
	for (auto const& adj_vertex : graph_.graph_vect_[vertex]) {
		adjacent_vertices.push_back(adj_vertex);
	}
	return adjacent_vertices;
}

void GraphAdjLists::Transpose() {
	std::vector <std::unordered_set<int>> transposed_graph_vect_;
	transposed_graph_vect_.resize(GetNumVertices());
	for (vertex_t vertex = 0; vertex < GetNumVertices(); ++vertex) {
		for (auto const& adj_vertex : graph_.graph_vect_[vertex]) {
			transposed_graph_vect_[adj_vertex].insert(vertex);
		}
	}
	graph_.graph_vect_ = transposed_graph_vect_;
}
