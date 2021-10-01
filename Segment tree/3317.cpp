#include <iostream>
#include <vector>
#include "segment_tree.h"
#include "operations.h"

void PrintSumm(size_t left_idx, size_t right_idx, const SegmentTree<int64_t, Sum<int64_t>>& segment_tree) {
	std::cout << segment_tree.Query(left_idx - 1, right_idx - 1) << " ";
}

void HandleQueries(SegmentTree<int64_t, Sum<int64_t>>& segment_tree) {
	char query;
	std::cin >> query;
	if (query == 's') {
		size_t left_idx, right_idx;
		std::cin >> left_idx >> right_idx;
		PrintSumm(left_idx, right_idx, segment_tree);
	} else if (query == 'u') {
		size_t idx;
		int64_t val;
		std::cin >> idx >> val;
		segment_tree.Update(idx - 1, val);
	}
}

int main() {
	size_t num_of_elements, num_of_queries;
	std::cin >> num_of_elements;
	std::vector<int64_t> numbers(num_of_elements);
	for (auto& number : numbers) {
		std::cin >> number;
	}
	std::cin >> num_of_queries;
	SegmentTree<int64_t, Sum<int64_t>> segment_tree(numbers, 0);
	for (size_t idx = 0; idx < num_of_queries; ++idx) {
		HandleQueries(segment_tree);
	}
}