#ifndef SPARSE_TABLE_H
#define SPARSE_TABLE_H

size_t Log2(size_t num) {
	size_t answer = 0;
	while (num >>= 1) {
		++answer;
	}
	return answer;
}

template <class T, class BinaryOp>
class SparseTable {
	std::vector<std::vector<T>> table_;
	BinaryOp op_;
public:
	explicit SparseTable(const std::vector<T>& data) {
		table_.push_back(data);
		for (size_t delta = 2; delta <= data.size(); delta *= 2) {
			std::vector<T> temp(data.size() - delta + 1);
			for (size_t idx = 0; idx < temp.size(); ++idx) {
				temp[idx] = op_(table_.back()[idx], table_.back()[idx + delta / 2]);
			}
			table_.push_back(std::move(temp));
		}
	}

	T Query(size_t left_idx, size_t right_idx) const {
		size_t log = Log2(right_idx - left_idx);
		size_t delta = 1 << log;
		T result = op_(table_[log][left_idx], table_[log][right_idx - delta + 1]);
		return result;
	}
};

#endif