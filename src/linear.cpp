#include "range_minimum_query/linear.h"
#include <climits>

namespace range_minimum_query {
	Linear::Linear(const std::vector<int>& a) : vec(a) { }

	int Linear::query(Range range) {
		if (range[0] >= range[1]) throw Invalid_Query_Exception("test");
		if (range[1] > vec.size()) throw std::out_of_range("test");
		int min = INT_MAX;
		for (size_t i = range[0]; i < range[1]; ++i) {
			min = std::min(min, vec[i]);
		}

		return min;
	}
}
