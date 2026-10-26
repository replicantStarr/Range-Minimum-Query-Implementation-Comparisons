#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	class Segment_Tree {
	public:
		Segment_Tree(const std::vector<int> a);
		~Segment_Tree();
		std::vector<int> Query(const std::vector<Query> queries);
	};
}
#endif
