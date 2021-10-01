#include <iostream>
#include <queue>
#include <vector>
#include <functional>

int IncreaseFirstDigit(int number) {
	if (number / 1000 != 9) {
		number += 1000;
	}
	return number;
}

int DecreaseLastDigit(int number) {
	if (number % 10 != 1) {
		number -= 1;
	}
	return number;
}

int ShiftDigitsToRight(int number) {
	 return (number % 10) * 1000 + number / 10;	
}

int ShiftDigitsToLeft(int number) {
	return (number % 1000) * 10 + number / 1000;
}

std::vector<int> FindNewNumbers(const int number) {
	std::vector<int> new_numbers;
	std::vector<std::function<int(int)>> functions = { IncreaseFirstDigit, DecreaseLastDigit, ShiftDigitsToRight, ShiftDigitsToLeft };
	for (auto& func : functions) {
		new_numbers.push_back(func(number));
	}
	return new_numbers;
}

std::vector<int> FindPath(const int from, const int to) {
	const int MAX = 10000;
	std::vector<bool> visited(MAX, false);
	std::vector<int> predecessors(MAX, -1);

	std::queue<int> order;
	order.push(from);
	visited[from] = true;

	while (!order.empty()) {
		auto num = order.front();
		order.pop();

		std::vector<int> new_numbers = FindNewNumbers(num);

		for (int next_number : new_numbers) {
			if (visited[next_number] != true) {
				visited[next_number] = true;
				order.push(next_number);
				predecessors[next_number] = num;
			}
		}
	}
	std::vector<int> route;
	int next = to;
	while (next != -1) {
		route.push_back(next);
		next = predecessors[next];
	}
	return { route.rbegin(), route.rend() };
}

void PrintNumbers(const std::vector<int>& numbers) {
	for (size_t number : numbers) {
		std::cout << number << std::endl;
	}
}

int main() {
	int from, to;
	std::cin >> from >> to;
	PrintNumbers(FindPath(from, to));
}