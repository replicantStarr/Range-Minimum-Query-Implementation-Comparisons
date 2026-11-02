#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/common.h"
#include <cmath>
#include <unordered_map>
#include <cstdint>

namespace range_minimum_query {

	// This is an implementation of the Fischer-Heun algorithm to solve RMQ, broken into 2 parts:
	// PRE-PROCESSING: the constructor
	// QUERYING: query
	//
	//
	// A high-level explanation for construction is as follows:
	// 	1. The array is passed
	// 	2. Divide it into blocks of size 1/2logn
	// 	3. Build a cartesian block -- a cartesian tree can be represented as a binary string 1010 where 1 is push and 0 is pop.
	// 	4. If the block already exists in hashmap do nothing
	// 	5. Otherwise, create a cartesian table for the block.
	// 	6. Add the index of that point to the summary.
	// 	7. Build a sparse table out of the summary.
	//
	// Then, the query works like:
	// 	1. Query cartesian tree of first block
	// 	2. Query cartesian tree of last block
	// 	3. Query sparse table for summary
	// 	4. Return result (These are constant time)


	// The constructor is the pre-processing phase of the algorithm.
	// It takes an array (vector in this case) of integers, and this becomes the only queryable data.  
	// To query newer data, a new class must be instantiated.
	//
	//
	

	void create_sparse_table(const int* i) {
		for (int _i = 0; _i < _i + block_size; ++_i) {
			sparse_table[_i] = (i + _i)*;
		}

		for (int j = 1; j < std::log2(block_size); ++j) {
			for (; i < i + block_size; ++i) {
				summary[i][j] = std::min(summary[i
			}
		}
	}

	std::pair<std::uint64_t, int> create_cartesian_number(const int* i, const int* j) {
		int min = INT_MAX;
		std::uint64_t cartesian_number = 0;
		std::stack<int> stack;

		for(; i < j; ++i) {
			min = std::min(min, i);
			while (!stack.empty() && stack.top() > i) {
				stack.pop();
				cartesian_number << 1;
			}

			stack.push(i);
			cartesian_number << 1;
			cartesian_number += 1;
		}

		return std::make_pair(cartesian_number, min);
	}

	Table create_cartesian_table(std::uint64_t cartesian_number, const int* i) {
		Table table(block.size(), std::vector<int>(block.size(), 0));

		for (int _i = 0; _i < i / block_size; ++_i) {
			table[_i][_i] = (i + _i)*;
			for (int j = i + 1; j < block_size; ++j) {
				table[_i][j] = std::min(table[_i][j-1], (i + j)*);
			}
		}

		cartesian_tables[cartesian_number] = table;
	}

	Fischer_Heun::Fischer_Heun(const std::vector<int>& a) {
		block_size = 0.5*std::log2(a.size());

		for (size_t i = 0; i < a.size(); i += b) {
			std::pair<std::uint64_t, int> cartesian_number = create_cartesian_number(&a[i], &a[b]);
			if (!cartesian_tables.contains(cartesian_number.first)) {
				create_cartesian_table(cartesian_number.first);
			}

			summary.push_back(cartesian_number.second);
		}

		create_sparse_table();
	}

	int Fischer_Heun::query([[maybe_unused]] Range range) {

	}
}
