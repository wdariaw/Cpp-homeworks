#include <iostream>
#include <queue>
#include <limits>
#include <vector>
#include <limits>
#include <string>

struct Position {
	int x_;
	int y_;
	int direction_;

	Position() {}

	Position(int x, int y, int direction) {
		x_ = x;
		y_ = y;
		direction_ = direction;
	}
};

void ReadMaze(std::vector<std::vector<bool>>& maze, size_t& start_x, size_t& start_y,
	size_t& finish_x, size_t& finish_y) {
	const size_t num_of_str = maze.size();
	const size_t num_of_col = maze[0].size();
	std::string str;
	std::getline(std::cin, str);
	for (size_t idx = 0; idx < num_of_str; ++idx) {
		std::getline(std::cin, str);
		for (size_t jdx = 0; jdx < num_of_col; ++jdx) {
			if (str[jdx] == ' ') {
				maze[idx][jdx] = true;
			}
			else if (str[jdx] == 'X') {
				maze[idx][jdx] = false;
			}
			else if (str[jdx] == 'S') {
				start_x = idx;
				start_y = jdx;
				maze[idx][jdx] = true;
			}
			else if (str[jdx] == 'F') {
				finish_x = idx;
				finish_y = jdx;
				maze[idx][jdx] = true;
			}
		}
	}
}

size_t FindDistance(const std::vector<std::vector<bool>>& maze, const size_t& start_x, const size_t& start_y,
	const size_t& finish_x, const size_t& finish_y) {
	const size_t num_of_str = maze.size();
	const size_t num_of_col = maze[0].size();
	std::vector<std::vector<std::vector<bool>>> visited(4, std::vector<std::vector<bool>>(num_of_str, std::vector<bool>(num_of_col, false)));
	size_t inf = std::numeric_limits<int>::max();
	std::vector<std::vector<std::vector<int>>> distances(4, std::vector<std::vector<int>>(num_of_str, std::vector<int>(num_of_col, inf)));
	std::queue<Position> order;
	const int num_of_directions = 4;
	for (size_t dir = 0; dir < num_of_directions; ++dir) {
		distances[dir][start_x][start_y] = 0;
		visited[dir][start_x][start_y] = true;
		order.push(Position(start_x, start_y, dir));
	}

	const std::vector<int> dx = { 0, 1, 0, -1 };
	const std::vector<int> dy = { 1, 0, -1, 0 };

	while (!order.empty()) {
		auto pos = order.front();
		order.pop();
		if (pos.x_ == finish_x && pos.y_ == finish_y) {
			return distances[pos.direction_][pos.x_][pos.y_];
		}
		Position new_pos;
		const int num_of_allowed_directions = 2;
		for (size_t dir_idx = 0; dir_idx < num_of_allowed_directions; ++dir_idx) {
			new_pos.direction_ = (pos.direction_ + dir_idx) % num_of_directions;
			new_pos.x_ = pos.x_ + dx[new_pos.direction_];
			new_pos.y_ = pos.y_ + dy[new_pos.direction_];
			if (!visited[new_pos.direction_][new_pos.x_][new_pos.y_] && maze[new_pos.x_][new_pos.y_]) {
				order.push(new_pos);
				visited[new_pos.direction_][new_pos.x_][new_pos.y_] = true;
				distances[new_pos.direction_][new_pos.x_][new_pos.y_] = distances[pos.direction_][pos.x_][pos.y_] + 1;
			}
		}
	}


}

int main() {
	size_t num_of_str, num_of_col, start_x, start_y, finish_x, finish_y;
	std::cin >> num_of_str >> num_of_col;
	std::vector<std::vector<bool>> maze(num_of_str, std::vector<bool>(num_of_col));
	ReadMaze(maze, start_x, start_y, finish_x, finish_y);
	std::cout << FindDistance(maze, start_x, start_y, finish_x, finish_y);
}