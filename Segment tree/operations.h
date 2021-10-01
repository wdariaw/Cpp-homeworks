#ifndef OPERATIONS_H
#define OPERATIONS_H

template <class T>
struct Max {
	std::pair<T, size_t> operator()(const std::pair<T, size_t>& lhs, const std::pair<T, size_t>& rhs) const {
		return std::max(lhs, rhs);
	}
};

template <class T>
struct Min {
	std::pair<T, size_t> operator()(const std::pair<T, size_t>& lhs, const std::pair<T, size_t>& rhs) const {
		return std::min(lhs, rhs);
	}
};

template <class T>
struct Sum {
	T operator()(const T& lhs, const T& rhs) const {
		return lhs + rhs;
	}
};

#endif