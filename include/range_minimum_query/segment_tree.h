#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	class Segment_Tree {
	public:
		Segment_Tree(const std::vector<int>& a);
		int query(Range range);
	private:
		int rangeMin(int pos, int nodeL, int nodeR, int queryL, int queryR);
		std::vector<int> segmentTree;
		std::size_t n;
		int leaves;
		
	};
}
