#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

int64_t ClosestPowerOfTwo(int64_t val) {
	int64_t power_of_two = 1;
	while (power_of_two < val) {
		power_of_two *= 2;
	}
	return power_of_two;
}

template <class T, class Func>
class SegmentTree {
	std::vector<T> tree_;
	Func op_;
	const T neutral_;
public:
	static size_t ParentIdx(size_t idx) {
		return idx / 2;
	}

	static size_t LeftIdx(size_t idx) {
		return idx * 2;
	}

	static size_t RightIdx(size_t idx) {
		return idx * 2 + 1;
	}

	SegmentTree(const std::vector<T>& data, const T& neutral)
		: tree_(2 * ClosestPowerOfTwo(data.size()), neutral), neutral_(neutral) {
		const size_t size = tree_.size();
		for (size_t idx = 0; idx < data.size(); ++idx) {
			tree_[idx + size / 2] = data[idx];
		}
		for (size_t idx = size / 2 - 1; idx > 0; --idx) {
			tree_[idx] = op_(tree_[LeftIdx(idx)], tree_[RightIdx(idx)]);
		}
	}

	T Query(size_t left_idx, size_t right_idx) const {
		T answer = neutral_;
		left_idx += tree_.size() / 2;
		right_idx += tree_.size() / 2;
		while (left_idx <= right_idx) {
			if (left_idx == RightIdx(ParentIdx(left_idx))) {
				answer = op_(answer, tree_[left_idx]);
			}
			if (right_idx == LeftIdx(ParentIdx(right_idx))) {
				answer = op_(answer, tree_[right_idx]);
			}
			left_idx = ParentIdx(left_idx + 1);
			right_idx = ParentIdx(right_idx - 1);
		}
		return answer;
	}

	void Update(size_t idx, const T& val) {
		idx += tree_.size() / 2;
		tree_[idx] = val;
		idx = ParentIdx(idx);
		while (idx > 0) {
			tree_[idx] = op_(tree_[LeftIdx(idx)], tree_[RightIdx(idx)]);
			idx = ParentIdx(idx);
		}
	}
};

#endif