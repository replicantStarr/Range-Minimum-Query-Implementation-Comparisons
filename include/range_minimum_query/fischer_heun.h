#pragma once

#include "range_minimum_query/common.h"
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace range_minimum_query {

	// Used to keep state for the fischer heun algorithm
	// Construction and querying are intentionally kept separate
	class Fischer_Heun {
	public:
		// Constructor is measure with array A
		Fischer_Heun(const std::vector<int>& a_);
		// The main query function, can take any amount of queries
		int query(Range range);
	
	private:
		std::pair<std::uint32_t, int> create_cartesian_number(size_t start, size_t end);
		void create_cartesian_table(size_t start, size_t end);
		void create_sparse_table();
		int linear_search(Range range);
		std::array<size_t,3> find_block_info(size_t i);

		std::vector<int> a;
		std::vector<int> summary;
		std::vector<std::vector<int>> sparse_table;
		std::vector<std::vector<int>> cartesian_tables;
		std::vector<std::uint32_t> cartesian_reference;

		size_t block_size;
	};

	size_t safe_log(size_t n);
}
