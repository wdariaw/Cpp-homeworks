#include <iostream>
#include <queue>
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

class GraphAdjLists : public Graph {
	std::vector<std::unordered_set<int>> graph_vect_;
	size_t num_of_edges_;
public:

	GraphAdjLists(vertex_t num_of_vertices);

	size_t GetNumVertices() const override;

	size_t GetNumEdges() const override;

	bool HasEdge(vertex_t from, vertex_t to) const  override;

	void AddEdge(vertex_t from, vertex_t to) override;

	void DeleteEdge(vertex_t from, vertex_t to) override;

	const std::vector<vertex_t> GetAdjacentVertices(vertex_t vertex) const override;

	void Transpose() override;

};

GraphAdjLists::GraphAdjLists(vertex_t num_of_vertices) {
	graph_vect_.resize(num_of_vertices);
	num_of_edges_ = 0;
}

size_t GraphAdjLists::GetNumVertices() const {
	return graph_vect_.size();
}

size_t GraphAdjLists::GetNumEdges() const {
	return num_of_edges_;
}

bool GraphAdjLists::HasEdge(vertex_t from, vertex_t to) const {
	if (graph_vect_[from].find(to) != graph_vect_[from].end()) {
		return true;
	}
	return false;
}

void GraphAdjLists::AddEdge(vertex_t from, vertex_t to) {
	graph_vect_[from].insert(to);
	++num_of_edges_;
}

void GraphAdjLists::DeleteEdge(vertex_t from, vertex_t to) {
	graph_vect_[from].erase(to);
	--num_of_edges_;
}

const std::vector<GraphAdjLists::vertex_t> GraphAdjLists::GetAdjacentVertices(vertex_t vertex) const {
	std::vector<vertex_t> adjacent_vertices;
	for (auto const& adj_vertex : graph_vect_[vertex]) {
		adjacent_vertices.push_back(adj_vertex);
	}
	return adjacent_vertices;
}

void GraphAdjLists::Transpose() {
	std::vector <std::unordered_set<int>> transposed_graph_vect_;
	transposed_graph_vect_.resize(GetNumVertices());
	for (vertex_t vertex = 0; vertex < GetNumVertices(); ++vertex) {
		for (auto const& adj_vertex : graph_vect_[vertex]) {
			transposed_graph_vect_[adj_vertex].insert(vertex);
		}
	}
	graph_vect_ = transposed_graph_vect_;
}



std::vector<size_t> FindPath(const GraphAdjLists& graph, int sourse, int dest) {
	const size_t numVertices = graph.GetNumVertices();
	std::vector<bool> visited(numVertices, false);
	std::vector<int> predecessors(numVertices, -1);
	std::queue<int> order;
	order.push(sourse);
	visited[sourse] = true;
	while (!order.empty()) {
		int vertex = order.front();
		order.pop();
		const std::vector<size_t> adj_vertexes = graph.GetAdjacentVertices(vertex);
		for (const size_t adj_vertex : adj_vertexes) {
			if (!visited[adj_vertex]) {
				visited[adj_vertex] = true;
				order.push(adj_vertex);
				predecessors[adj_vertex] = vertex;
			}
		}
	}

	if (!visited[dest]) {
		return std::vector<size_t>();
	}
	std::vector<size_t> route;
	int next = dest;
	while (next != -1) {
		route.push_back(next + 1);
		next = predecessors[next];
	}
	return { route.rbegin(), route.rend() };
}

void FillGraph(GraphAdjLists& graph, size_t num_of_vertexes) {
	bool adj;
	for (size_t from = 0; from < num_of_vertexes; ++from) {
		for (size_t to = 0; to < num_of_vertexes; ++to) {
			std::cin >> adj;
			if (adj) {
				graph.AddEdge(from, to);
			}
		}
	}
}

void PrintPath(const std::vector<size_t>& path) {
	if (path.size() == 1) {
		std::cout << 0 << std::endl;
	} else if (path.size() != 0) {
		std::cout << path.size() - 1 << std::endl;
		for (auto const& vertex : path) {
			std::cout << vertex << " ";
		}
	} else {
		std::cout << -1;
	}
}

int main() {
	size_t num_of_vertexes;
	std::cin >> num_of_vertexes;
	GraphAdjLists graph(num_of_vertexes);
	FillGraph(graph, num_of_vertexes);
	size_t from, to;
	std::cin >> from >> to;
	const std::vector<size_t> path = FindPath(graph, from - 1, to - 1);
	PrintPath(path);
}