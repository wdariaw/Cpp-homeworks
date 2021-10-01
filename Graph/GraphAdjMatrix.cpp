#include "graph.h"

GraphAdjMatrix::GraphAdjMatrix(vertex_t num_of_vertices) {
	graph_.graph_matrix_.assign(num_of_vertices, std::vector<bool>(num_of_vertices));
	graph_.num_of_edges_ = 0;
}

size_t GraphAdjMatrix::GetNumVertices() const {
	return graph_.graph_matrix_.size();
}

size_t GraphAdjMatrix::GetNumEdges() const {
	return graph_.num_of_edges_;
}

bool GraphAdjMatrix::HasEdge(vertex_t from, vertex_t to) const {
	if (graph_.graph_matrix_[from][to]) {
		return true;
	}
	return false;
}

void GraphAdjMatrix::AddEdge(vertex_t from, vertex_t to) {
	graph_.graph_matrix_[from][to] = true;
	++graph_.num_of_edges_;
}

void GraphAdjMatrix::DeleteEdge(vertex_t from, vertex_t to) {
	graph_.graph_matrix_[from][to] = false;
	--graph_.num_of_edges_;
}

const std::vector<GraphAdjMatrix::vertex_t> GraphAdjMatrix::GetAdjacentVertices(vertex_t vertex) const {
	std::vector<vertex_t> adjacent_vertices;
	for (vertex_t vertex_idx = 0; vertex_idx < GetNumVertices(); ++vertex_idx) {
		if (graph_.graph_matrix_[vertex][vertex_idx]) {
			adjacent_vertices.push_back(vertex_idx);
		}
	}
	return adjacent_vertices;
}

void GraphAdjMatrix::Transpose() {
	std::vector<std::vector<bool>> transposed_graph_matrix_;
	vertex_t num_of_vertices = GetNumVertices();
	transposed_graph_matrix_.assign(num_of_vertices, std::vector<bool>(num_of_vertices));
	for (vertex_t vertex_idx = 0; vertex_idx < num_of_vertices; ++vertex_idx) {
		for (vertex_t vertex_jdx = 0; vertex_jdx < num_of_vertices; ++vertex_jdx) {
			transposed_graph_matrix_[vertex_jdx][vertex_idx] = graph_.graph_matrix_[vertex_idx][vertex_jdx];
		}
	}
	graph_.graph_matrix_ = transposed_graph_matrix_;
}
