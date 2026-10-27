#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	class Segment_Tree {
	public:
		Segment_Tree(const std::vector<int>& a);
		std::vector<int> query(const std::vector<Query>& queries);
	private:
		std::vector<int> segmentTree;
		int leaves;
	};
}
