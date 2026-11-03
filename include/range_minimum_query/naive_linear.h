#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {
	class Naive_Linear {
	public:
		Naive_Linear(const std::vector<int>& a);
		int query(Range range);
	private:
		// Only need to store the vector
		const std::vector<int>& vec;
	};
}
