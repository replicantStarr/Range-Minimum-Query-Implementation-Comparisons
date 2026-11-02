#pragma once

#include "range_minimum_query/common.h"

namespace range_minimum_query {
	class Linear {
	public:
		Linear(const std::vector<int>& a);
		int query(Range range);
	private:
		std::vector<int> vec;
	}
}
