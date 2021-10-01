#include <iostream>
#include <queue>
#include <limits>
#include <vector>

void ReadStartPoints(std::vector<std::pair<int, int>>& start_points, const size_t num_of_str, const size_t num_of_col) {
	bool adj;
	for (size_t idx = 0; idx < num_of_str; ++idx) {
		for (size_t jdx = 0; jdx < num_of_col; ++jdx) {
			std::cin >> adj;
			if (adj) {
				start_points.push_back(std::make_pair(idx, jdx));
			}
		}
	}
}

std::vector<std::vector<int>> FindDistances(std::vector<std::pair<int, int>>& start_points, const size_t num_of_str, const size_t num_of_col) {
	std::vector<std::vector<bool>> visited(num_of_str, std::vector<bool>(num_of_col, false));
	size_t inf = std::numeric_limits<int>::max();
	std::vector<std::vector<int>> distances(num_of_str, std::vector<int>(num_of_col, inf));

	std::queue<std::pair<int, int>> order;
	for (const auto& start : start_points) {
		order.push(start);
		distances[start.first][start.second] = 0;
		visited[start.first][start.second] = true;
	}

	while (!order.empty()) {
		auto square = order.front();
		order.pop();

		const std::vector<int> dx = { 1, 0, -1, 0 };
		const std::vector<int> dy = { 0, 1, 0, -1 };

		for (size_t adj_square_idx = 0; adj_square_idx < dx.size(); ++adj_square_idx) {
			std::pair<int, int> new_square = std::make_pair(square.first + dx[adj_square_idx], square.second + dy[adj_square_idx]);
			if (new_square.first >= 0 && new_square.second >= 0 && new_square.first < num_of_str &&
				new_square.second < num_of_col && !visited[new_square.first][new_square.second]) {
				visited[new_square.first][new_square.second] = true;
				order.push(new_square);
				distances[new_square.first][new_square.second] = distances[square.first][square.second] + 1;
			}
		}
	}
	return distances;
}

void PrintTable(const std::vector<std::vector<int>>& distances) {
	const size_t num_of_str = distances.size();
	const size_t num_of_col = distances[0].size();
	for (size_t idx = 0; idx < num_of_str; ++idx) {
		for (size_t jdx = 0; jdx < num_of_col; ++jdx) {
			std::cout << distances[idx][jdx] << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	size_t num_of_str, num_of_col;
	std::cin >> num_of_str >> num_of_col;
	std::vector<std::pair<int, int>> start_points;
	ReadStartPoints(start_points, num_of_str, num_of_col);
	PrintTable(FindDistances(start_points, num_of_str, num_of_col));
}