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


	// A simple function which will round the log2, and give 0 for 0 instead of infinite
	size_t safe_log(size_t n) {
		if (n == 0) return 0;
		return std::floor(std::log2(n));
	}

	// Creating a cartesian tree is just a sequence of pushing and popping a stack.
	// This can be represented in binary as a number.  If anything has the same number, they have the same indicies of smallest.
	std::pair<std::uint32_t, int> Fischer_Heun::create_cartesian_number(size_t start, size_t end) {
		// Whilst processing, find the minimum value of the block.
		int min = a[start];
		std::uint32_t cartesian_number = 0;

		// A stack will make it simple to determine how many pops before push.
		std::vector<int> stack;
		stack.reserve(end - start);

		// Loop over the block.
		for(size_t i = start; i < end; ++i) {
			// Take min block value
			min = std::min(min, a[i]);

			while (!stack.empty() && stack.back() > a[i]) {
				// All numbers are binary, so a simple bitshift will mean pop (0)
				stack.pop_back();
				cartesian_number <<= 1;
			}

			// Push the new value to top of stack
			stack.push_back(a[i]);

			// Same as before, expect the bitshift adds a one.
			cartesian_number = (cartesian_number << 1) | 1;
		}

		return {cartesian_number, min};
	}

	// A cartesian table is used to return the index of the smallest value on a range specific to a cartesian tree.
	void Fischer_Heun::create_cartesian_table(size_t start, size_t end) {
		// The table is a flat n*n, this is for memory optimisation.
		std::vector<int> table(block_size * block_size, 0);
		size_t dif = end - start;

		for (size_t i = 0; i < dif; ++i) {
			// i * block_size is the block the respective block i.e. 2 * block_size is 3rd block.
			// + i is the shift inside the block, i.e. 3rd block 3rd item.
			table[i * block_size + i] = i;
			for (size_t j = i + 1; j < dif; ++j) {
				// The value in the table before I.e 3rd block 2nd item.
				int previous = table[i * block_size + j - 1];
				// This is a DP trick, where if at any point it's the min of current and previous,
				// Then just take the min of last or current.  Which is exactly this.
				table[i * block_size + j] = a[start + j] < a[start + previous] ? j : previous;
			}
		}

		// The actual table is pushed to field, and is given full permission instead of copying.
		cartesian_tables.push_back(std::move(table));
	}

	// A sparse table is created over the summary values of the vector.
	void Fischer_Heun::create_sparse_table() {
		// Creates the sparse table as summary * log2 summary.
		sparse_table.assign(summary.size(), std::vector<int>(safe_log(summary.size()) + 1));

		// First items looped as 2^0 = 1
		for (size_t i = 0; i < summary.size(); ++i) {
			sparse_table[i][0] = i;
		}

		// Loop as powers of 2, 2^j 
		for (size_t j = 1; summary.size() >= (size_t(1) << j); ++j) {
			// The previous power, i.e j = 2 then k = 1
			size_t k = size_t(1) << (j - 1);

			// The table must be entirely covered by powers of 2.
			for (size_t i = 0; summary.size() >= i + 2*k; ++i) {
				// Simple DP, take the min.
				int v1 = sparse_table[i][j - 1];
				int v2 = sparse_table[i + k][j - 1];
				sparse_table[i][j] = summary[v1] < summary[v2] ? v1 : v2;
			}
		}
	}

	// The constructor acts as the pre-processing phase, responsible for building essential fields.
	Fischer_Heun::Fischer_Heun(const std::vector<int>& a_) : Fischer_Heun(a_, 0.5*safe_log(a_.size()))

	Fischer_Heun::Fischer_Heun(const std::vector<int>& a_, size_t block_size) 
		a(a_), 
		block_size(block_size) 
	{
		// If the array size is 0, nothing is to be constructed
		// If the block size is < 2, a linear search is more optimal
		if (a.size() == 0 || block_size < 2) return;

		// Tracks the cartesian tables which have been seen (this is an optimisation to prevent hashing on query)
		// Cartesian number must be double the block size, so being uint32_t it can store any array size up to 2^32.
		std::unordered_map<std::uint32_t, std::uint32_t> existing_cartesians;

		// Loop over each block in the array.
		for (size_t i = 0; i < a.size(); i += block_size) {
			// Either to the end of the block, or end of array if block is too big.
			size_t j = std::min(i + block_size, a.size());

			// create a cartesian number on the block, and find the minimum value of the block.
			std::pair<std::uint32_t, int> cartesian_number = create_cartesian_number(i, j);

			// Iterate and try find the cartesian number.
			auto it = existing_cartesians.find(cartesian_number.first);
			if (it == existing_cartesians.end()) {
				// If not found, create a cartesian table and store it at the start.
				it = existing_cartesians.emplace(cartesian_number.first, cartesian_tables.size()).first;
				create_cartesian_table(i, j);
			}

			// Each index of a block contains a reference to a cartesian number now.
			cartesian_reference.push_back(it->second);

			// Store the min of each block in summary
			summary.push_back(cartesian_number.second);
		}

		// Once all blocks are complete, create a sparese table.
		create_sparse_table();
	}

	// Simple linear search
	int Fischer_Heun::linear_search(Range range) {
		int min = a[range[0]];
		for (size_t i = range[0]; i <= range[1]; ++i) {
			min = std::min(min, a[i]);
		}

		return min;
	}

	// Simply store the block information
	std::array<size_t,3> Fischer_Heun::find_block_info(size_t i) {
		return {
			i / block_size, // Block
			i % block_size, // Index in Block
			(i / block_size) * block_size // Next block
		};
	}

	// This is the querying function, range is both inclusive and the overall runtime is constant
	// Small cases are linear but it's too small to consider.
	int Fischer_Heun::query(Range range) {
	    if (range[0] > range[1]) throw Invalid_Query_Exception("range[0] is > range[1]");
	    if (range[1] >= a.size())  throw std::out_of_range("range[1] is greater than the size of the original array");

	    if (block_size < 2) return linear_search(range);

	    // Get block information
	    std::array<size_t,3> s_block = find_block_info(range[0]);
	    std::array<size_t,3> e_block = find_block_info(range[1]);

	    // Find the cartesian tables based on the blocks
	    const std::vector<int>& s_table = cartesian_tables[cartesian_reference[s_block[0]]];
	    const std::vector<int>& e_table = cartesian_tables[cartesian_reference[e_block[0]]];

	    // Same block means the min is just the min of the cartesian table
	    if (s_block[0] == e_block[0]) return a[s_block[2] + s_table[s_block[1] * block_size + e_block[1]]];

	    // Find the min between both cartesian tables
	    int res = std::min(a[s_block[2] + s_table[s_block[1] * block_size + block_size -1]], 
			    a[e_block[2] + e_table[e_block[1]]]);

	    // If there is only 1 difference, no need for summary sparse table
	    if (s_block[0] + 1 < e_block[0]) {
		    // Bump because no need for cartesian blocks
		    size_t S = s_block[0] + 1;
		    size_t E = e_block[0] - 1;
		    // The width of the difference
		    size_t w = std::bit_width(E - S + 1) - 1;
		    res = std::min({res, summary[sparse_table[S][w]], summary[sparse_table[E - (size_t(1) << w) + 1][w]]});
	    }

	    return res;
	}
}
