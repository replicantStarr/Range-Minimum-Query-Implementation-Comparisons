#ifndef SPARSE_TABLE_H 
#define SPARSE_TABLE_H

#include "range_minimum_query/common.h"
#include <vector>

namespace range_minimum_query {

	class Sparse_Table {
	public:
		Sparse_Table(const std::vector<int> a);
		~Sparse_Table();
		std::vector<int> Query(const std::vector<Query> queries);
	};
}
#endif
