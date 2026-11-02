#include "range_minimum_query/linear.h"
#include <climits>

namespace range_minimum_query {
	Linear::Linear(const std::vector<int>& a) : vec(a) { }

	int Linear::query(Range range) {
		int min = INT_MAX;
		for (size_t i = range[0]; i < range[1]; ++i) {
			min = std::min(min, vec[i]);
		}

		return min;
	}
}
