#include "range_minimum_query/segment_tree.h"
#include <algorithm>
#include <climits>

namespace range_minimum_query {

	Segment_Tree::Segment_Tree(const std::vector<int>& a) {

		//Initialising the size of the segment tree
		int inputSize{ static_cast<int>(a.size()) };
		int leaves{ 1 };

		while (leaves < inputSize) leaves *= 2;

		segmentTree = std::vector<int>(leaves * 2 - 1, INT_MAX);
		

	}

	std::vector<int> Segment_Tree::query([[maybe_unused]] const std::vector<Query>& queries) {
		return {0};
	}
}
