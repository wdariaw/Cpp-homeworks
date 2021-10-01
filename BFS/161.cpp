#include <iostream>
#include <queue>
#include <vector>

std::vector<std::pair<int, int>> FindPath(const size_t size, std::pair<int, int> sourse, std::pair<int, int> dest) {
	std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));
	std::vector<std::vector<std::pair<int, int>>> predecessors(size, std::vector<std::pair<int, int>>(size, std::make_pair(-1, -1)));
	std::queue<std::pair<int, int>> order;

	order.push(sourse);
	visited[sourse.first][sourse.second] = true;

	while (!order.empty()) {
		auto square = order.front();
		order.pop();

		const std::vector<int> dx = { 2, 2, 1, 1, -1, -1, -2, -2 };
		const std::vector<int> dy = { 1, -1, 2, -2, 2, -2, 1, -1 };

		for (size_t adj_square_idx = 0; adj_square_idx < dx.size(); ++adj_square_idx) {
			std::pair<int, int> new_square = std::make_pair(square.first + dx[adj_square_idx], square.second + dy[adj_square_idx]);
			if (new_square.first >= 0 && new_square.second >= 0 && new_square.first < size &&
				new_square.second < size && !visited[new_square.first][new_square.second]) {
				visited[new_square.first][new_square.second] = true;
				order.push(new_square);
				predecessors[new_square.first][new_square.second] = square;
			}
		}
	}
	std::vector<std::pair<int, int>> route;
	auto next = dest;
	while (next.first != -1 && next.second != -1) {
		route.push_back(next);
		next = predecessors[next.first][next.second];
	}
	return { route.rbegin(), route.rend() };
}

void PrintPath(const std::vector<std::pair<int, int>>& path) {
	std::cout << path.size() - 1 << std::endl;
	for (auto const& square : path) {
		std::cout << square.first + 1 << " " << square.second + 1 << " " << std::endl;
	}
}

int main() {
	size_t size;
	std::cin >> size;
	std::pair<int, int> from, to;
	std::cin >> from.first >> from.second;
	std::cin >> to.first >> to.second;
	const std::vector<std::pair<int, int>> path = FindPath(size, std::make_pair<int, int>(from.first - 1, from.second - 1), std::make_pair<int, int>(to.first - 1, to.second - 1));
	PrintPath(path);
}