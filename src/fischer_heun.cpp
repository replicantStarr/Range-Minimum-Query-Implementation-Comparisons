#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/common.h"
#include <cmath>
#include <unordered_map>
#include <cstdint>
#include <stack>
#include <climits>
#include <iostream>

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
	

	void Fischer_Heun::create_sparse_table() {
		std::cout << "creating table" << std::endl;
		for (size_t i = 0; i < summary.size(); ++i) {
			sparse_table[i][0] = i;
		}
		std::cout << "sparse initialised" << std::endl;

		for (int j = 1; j < std::log2((int)summary.size()); ++j) {
			for (int i = 0; i < j; ++i) {
				sparse_table[i][j] = summary[sparse_table[i - j][j - 1]] < summary[sparse_table[i][j - 1]] 
					? sparse_table[i - j][j - 1]
					: sparse_table[i][j - 1];
			}
		}
	}

	std::pair<std::uint64_t, int> Fischer_Heun::create_cartesian_number(int i, int j) {
		int min = INT_MAX;
		std::uint64_t cartesian_number = 0;
		std::stack<int> stack;

		for(; i < j; ++i) {
			while (!stack.empty() && stack.top() > a[i]) {
				stack.pop();
				cartesian_number <<= 1;
			}

			stack.push(a[i]);
			cartesian_number <<= 1;
			cartesian_number += 1;
		}

		return std::make_pair(cartesian_number, min);
	}

	void Fischer_Heun::create_cartesian_table(const std::uint64_t cartesian_number, int i, int j) {
		Table table(block_size, std::vector<int>(block_size, 0));

		for (; i < j; ++i) {
			table[i][i] = i;
			for (int j_ = i + 1; j_ < j; ++j_) {
				table[i][j_] = a[table[i][j_ - 1]] > a[j_] ? j_ : table[i][j_-1];
			}
		}

		cartesian_tables[cartesian_number] = table;
	}

	Fischer_Heun::Fischer_Heun(const std::vector<int>& a_) : sparse_table(a.size(), std::vector<int>(std::log2(a.size()), 0)), a(a_), block_size(0.5*std::log2(a.size())) {
		for (size_t i = 0; i < a.size(); i += block_size) {
			std::cout << i << std::endl;
			size_t j = i + block_size > a.size() ? a.size() : i + block_size;
			std::pair<std::uint64_t, int> cartesian_number = create_cartesian_number(i, j);
			std::cout << "cartesian number created" << std::endl;

			if (!cartesian_tables.contains(cartesian_number.first)) create_cartesian_table(cartesian_number.first, i, j);
			std::cout << "created table" << std::endl;

			summary.push_back(cartesian_number.second);
			std::cout << "added to summary" << std::endl;
			cartesian_numbers.push_back(cartesian_number.first);
			std::cout << "added to numbers" << std::endl;
		}

		create_sparse_table();
		std::cout << "sparse table created" << std::endl;
	}

	int Fischer_Heun::query(Range range) {
		if (range[0] >= range[1]) throw Invalid_Query_Exception("Test");
		if (range[1] >= a.size()) throw std::out_of_range("Test");

		return std::min(sparse_table[(range[0] + block_size) / block_size][std::log2(range[0] - range[1])], 
				std::min(cartesian_tables[cartesian_numbers[range[0] / block_size]][range[0] % block_size][block_size - 1],
					cartesian_tables[cartesian_numbers[range[1] / block_size]][0][range[1] % block_size]));
	}
}
