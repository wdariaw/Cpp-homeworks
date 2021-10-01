#include <iostream>
#include <vector>
#include <algorithm>
#include "operations.h"
#include "sparse_table.h"

void PrintMax(size_t left_idx, size_t right_idx, const SparseTable<std::pair<int64_t, size_t>, Max<int64_t>>& table) {
	std::cout << table.Query(left_idx, right_idx).first << " " << table.Query(left_idx, right_idx).second << "\n";
}

int main() {
	size_t num_of_elements, num_of_queries, left_idx, right_idx;
	std::cin >> num_of_elements;
	std::vector<std::pair<int64_t, size_t>> numbers(num_of_elements);
	for (size_t idx = 0; idx < num_of_elements; ++idx) {
		std::cin >> numbers[idx].first;
		numbers[idx].second = idx + 1;
	}
	std::cin >> num_of_queries;
	SparseTable<std::pair<int64_t, size_t>, Max<int64_t>> table(numbers);
	for (size_t idx = 0; idx < num_of_queries; ++idx) {
		std::cin >> left_idx >> right_idx;
		PrintMax(left_idx - 1, right_idx - 1, table);
	}
}