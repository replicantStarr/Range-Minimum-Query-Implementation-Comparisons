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
		Fischer_Heun(const std::vector<int>& a_, size_t block_size);
		friend struct Fischer_Heun_Benchmark;

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

	// This is an exclusive class only allowed in benchmarking to demonstrate different block sizes,
	// This is not used in any source code and is designed to be mostly stateless.
	struct Fischer_Heun_Benchmark {
		static Fischer_Heun create(const std::vector<int>& a, size_t block_size) {
			return Fischer_Heun(a, block_size);
		}

		static size_t block_size(const Fischer_Heun& fh) { return fh.block_size; };
		static size_t table_count(const Fischer_Heun& fh) { return fh.cartesian_tables.size(); };
	};

	size_t safe_log(size_t n);
}
