#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	class Sparse_Table {
	public:
		Sparse_Table(const std::vector<int>& a);
		std::vector<int> query(const std::vector<Query>& queries);
	};
}
