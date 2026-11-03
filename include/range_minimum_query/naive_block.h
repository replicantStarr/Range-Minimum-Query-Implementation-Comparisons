#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {
	class Naive_Block {
	public:
		Naive_Block(const std::vector<int>& a);
		int query(Range range);

	private:
		// Only require a 2D vector of each possible query combination
		std::vector<std::vector<int>> m;
	};
}
