#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	class Segment_Tree {
	public:
		Segment_Tree(const std::vector<int>& a);
		int query(Range range);
}
