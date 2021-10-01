#include <iostream>
#include <algorithm>
#include <string>
#include "graph.h"

void DFS(int vertex, std::vector<bool>& visited, const GraphAdjLists& graph) {
	visited[vertex] = true;
	size_t num_of_adj_vert = graph.GetAdjacentVertices(vertex).size();
	for (size_t adj_vert = 0; adj_vert < num_of_adj_vert; ++adj_vert) {
		int to = graph.GetAdjacentVertices(vertex)[adj_vert];
		if (!visited[to]) {
			DFS(to, visited, graph);
		}
	}
}


void FillPaper(std::vector<std::vector<bool>>& paper, size_t str, size_t col) {
	paper.assign(str, std::vector<bool>(col));
	std::string symb_str;
	std::getline(std::cin, symb_str);
	for (size_t idx = 0; idx < str; ++idx) {
		std::getline(std::cin, symb_str);
		for (size_t jdx = 0; jdx < col; ++jdx) {
			if (symb_str[jdx] == '#') {
				paper[idx][jdx] = true;
			}
			else {
				paper[idx][jdx] = false;
			}
		}
	}
}

void FillGraph(GraphAdjLists& graph, const std::vector<std::vector<bool>>& paper) {
	size_t str, col;
	str = paper.size();
	col = paper[0].size();
	for (size_t idx = 0; idx < str; ++idx) {
		for (size_t jdx = 0; jdx < col; ++jdx) {
			if (paper[idx][jdx] && (idx != 0) && paper[idx - 1][jdx]) {
				graph.AddEdge((idx - 1) * col + jdx, idx * col + jdx);
				graph.AddEdge(idx * col + jdx, (idx - 1) * col + jdx);
			}
			if (paper[idx][jdx] && (jdx != 0) && paper[idx][jdx - 1]) {
				graph.AddEdge(idx * col + jdx, idx * col + jdx - 1);
				graph.AddEdge(idx * col + jdx - 1, idx * col + jdx);
			}
		}
	}
}

size_t FindNumOfPieces(std::vector<std::vector<bool>> paper) {
	size_t str, col;
	str = paper.size();
	col = paper[0].size();
	size_t num_of_vert = str * col;
	GraphAdjLists graph(num_of_vert);
	FillGraph(graph, paper);
	std::vector<bool> visited;
	visited.assign(num_of_vert, false);
	size_t num_of_pieces = 0;
	for (int vertex = 0; vertex < num_of_vert; ++vertex) {
		if (paper[vertex / col][vertex % col] && !visited[vertex]) {
			DFS(vertex, visited, graph);
			++num_of_pieces;
		}
	}
	return num_of_pieces;
}

int main() {
	size_t str, col;
	std::cin >> str >> col;
	std::vector<std::vector<bool>> paper;
	FillPaper(paper, str, col);
	std::cout << FindNumOfPieces(paper);
}