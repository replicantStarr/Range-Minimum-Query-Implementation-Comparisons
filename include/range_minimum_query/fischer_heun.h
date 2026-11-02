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
		using Table = std::vector<std::vector<int>>;

		void create_sparse_table();
		std::pair<std::uint64_t, int> create_cartesian_number(int i, int j);
		void create_cartesian_table(const std::uint64_t cartesian_number, int i, int j);

		std::unordered_map<std::uint64_t, Table> cartesian_tables;
		std::vector<std::uint64_t> cartesian_numbers;
		Table sparse_table;
		std::vector<int> summary;
		std::vector<int> a;

		size_t block_size;
	};
}
