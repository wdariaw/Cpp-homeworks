#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <cstdlib>
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


int FindRemainder(const std::string& dividend_str, int divisor) {
	int remainder = 0;
	for (char digit : dividend_str) {
		remainder = (remainder * 10 + atoi(&digit)) % divisor;
	}
	return remainder;
}

std::string FindMinMultiple(const std::string& base_num, const int div, std::vector<int>& digits) {
	int remainder = FindRemainder(base_num, div);
	GraphAdjLists graph(div);
	for (int rem = 0; rem < div; ++rem) {
		for (const auto& digit : digits) {
			int new_rem = (rem * 10 + digit) % div;
			graph.AddEdge(new_rem, rem);
		}
	}
	size_t inf = std::numeric_limits<int>::max();
	std::vector<int> distances(div, inf);
	std::vector<bool> visited(div, false);
	std::queue<int> order;
	order.push(0);
	visited[0] = true;
	distances[0] = 0;
	while (!order.empty()) {
		int cur = order.front();
		order.pop();
		const std::vector<size_t> previous_remainders = graph.GetAdjacentVertices(cur);
		for (const int& rem : previous_remainders) {
			if (!visited[rem]) {
				visited[rem] = true;
				distances[rem] = distances[cur] + 1;
				order.push(rem);
			}
		}
	}

	if (!visited[remainder]) {
		return "-1";
	}

	std::sort(digits.begin(), digits.end());
	std::string multiple(base_num);
	while (remainder != 0) {
		for (const int& digit : digits) {
			int new_rem = (remainder * 10 + digit) % div;
			if (distances[new_rem] == distances[remainder] - 1) {
				remainder = new_rem;
				multiple += std::to_string(digit);
				break;
			}
		}
	}
	return multiple;
}

int main() {
	std::string base_num;
	int div;
	std::cin >> base_num;
	std::cin >> div;
	size_t num_of_digits;
	std::cin >> num_of_digits;
	std::vector<int> digits(num_of_digits);
	for (auto& digit : digits) {
		std::cin >> digit;
	}
	std::cout << FindMinMultiple(base_num, div, digits);
}