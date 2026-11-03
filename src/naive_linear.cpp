#include "range_minimum_query/naive_linear.h"
#include <climits>

namespace range_minimum_query {
	// Simple just query the array
	Naive_Linear::Naive_Linear(const std::vector<int>& a) : vec(a) { }

	// The query is just a simple loop finding the minimum, runs in O(n)
	int Naive_Linear::query(Range range) {
		if (range[0] > range[1]) throw Invalid_Query_Exception("test");
		if (range[1] >= vec.size()) throw std::out_of_range("test");
		int min = INT_MAX;
		for (size_t i = range[0]; i <= range[1]; ++i) {
			min = std::min(min, vec[i]);
		}

		return min;
	}
}
