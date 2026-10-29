#pragma once

#include "range_minimum_query/common.h"
#include <vector>
#include <unordered_map>

namespace range_minimum_query {

	// Used to keep state for the fischer heun algorithm
	// Construction and querying are intentionally kept separate
	class Fischer_Heun {
	public:
		// Constructor is measure with array A
		Fischer_Heun(const std::vector<int>& a);
		// The main query function, can take any amount of queries
		int query(Range range);
	
	private:
		using Table = std::vector<std::vector<int>>;
		std::unordered_map<std::string, Table>
	};
}
