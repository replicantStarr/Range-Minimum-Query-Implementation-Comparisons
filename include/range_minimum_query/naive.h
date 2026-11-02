#pragma once

#include "range_minimum_query/common.h"

namespace range_minimum_query {
	class Naive {
	public:
		Naive(const std::vector<int> a);
		int query(Range range);

	private:
		std::vector<std::vector<int>> m;
	}
}
