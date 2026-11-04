#pragma once

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	class Sparse_Table {
	public:
		Sparse_Table(const std::vector<int>& a);
		int query(Range range);

	private:
		std::vector<std::vector<int>> sparseTable;
		std::vector<size_t> logTable;
		size_t n;
	};
}
