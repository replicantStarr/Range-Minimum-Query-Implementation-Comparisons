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


	// Given a vector and block size, it will return the lowest value per block.
	std::vector<int> create_summary(const std::vector<int>& a) {
		std::vector<int> summary;
		for (size_t i = 0; i < a.size(); i += block_size) {
			int m = INT_MAX;
			for (size_t j = i; j < i + block_size; ++j) {
				m = std::min(m, a[j]);
			}

			summary.push_back(m);
		}

		return summary;
	}

	// now creates a sparse table based on the summary to allow for quick lookup
	std::vector<std::vector<int>> create_sparse_table(const std::vector<std::vector<int>>& summary) {
		std::vector<std::vector<int>> sparse_table(summary.size(), std::vector<int>(block_size, std::vector<int>));

		for (size_t i = 0; i < summary.size(); ++i) {
			for (size_t j = 0; j < block_size; ++j) {
				
			}
		}
	}

	std::uint64_t create_cartesian_number(int i, int j,
			const std::vector<int>& block) {
		std::stack<int> stack;
		std::string cartesian_number = "";

		for (size_t _i = i; _i < j; ++i) {
			if (stack.empty()) {
				stack.push(block[_i]);
				cartesian_number += "1";
				continue;
			}

			while (stack.top() > block[_i]) {
				stack.pop();
				cartesian_number += "0";
			}
1
			stack.push(block[_i]);
			cartesian_number += "1";
		}

		return cartesian_number;
	}

	std::uint64_t create_cartesian_number(const int* i, const int* j) {
		std::uint64_t cartesian_number = 1;
		std::stack<int> stack;
		stack.push(i);
		++i;

		for(; i < j; ++i) {
			while (stack.top() > i) {
				stack.pop();
				cartesian_number << 1;
			}

			stack.push(i);
			cartesian_number << 1;
			cartesian_number += 1;
		}

		return cartesian_number;
	}

	Table create_cartesian_table(const std::vector<int>& block) {
		Table table(block.size(), std::vector<int>(block.size(), INT_MAX));
		for (size_t i = 0; i < block.size(); ++i) {
			for (size_t j = i; j < block.size(); ++j) {
				table[i][j] = std::min(table[i][j-1], block[j]);
			}
		}

		return table;
	}

	Fischer_Heun::Fischer_Heun(const std::vector<int>& a) {
		// derived as the most optimal block size
		block_size = 0.5*std::log2(a.size());

		for (size_t i = 0; i < a.size(); i += b) {
			build_cartesian_block();
			build_sparse_table();

		}
	}

	int Fischer_Heun::query([[maybe_unused]] Range range) {
		return 0;
	}
}
