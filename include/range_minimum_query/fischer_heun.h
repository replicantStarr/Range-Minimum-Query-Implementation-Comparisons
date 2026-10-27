#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	// Used to keep state for the fischer heun algorithm
	// Construction and querying are intentionally kept separate
	class Fischer_Heun {
	public:
		// Constructor is measure with array A
		Fischer_Heun(const std::vector<int>& a);
		// The main query function, can take any amount of queries
		std::vector<int> query(const std::vector<Query>& queries);
	};
}
