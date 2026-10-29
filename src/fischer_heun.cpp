#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/common.h"
#include <cmath>
#include <unordered_map>

namespace range_minimum_query {

	// This is an implementation of the Fischer-Heun algorithm to solve RMQ, broken into 2 parts:
	// PRE-PROCESSING: the constructor
	// QUERYING: query
	//
	//
	// A high-level explanation for construction is as follows:
	// 	1. The array is passed
	// 	2. Divide it into blocks 
	// 	3. Build a cartesian block
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
	std::vector<int> create_summary(int block, 
			const std::vector<int>& a) {

	}

	// now creates a sparse table based on the summary to allow for quick lookup
	std::vector<std::vector<int>> create_sparse_table(const std::vector<std::vector<int>>& summary) {
		return {};
	}

	std::string create_cartesian_number(const std::vector<int>& block) {
		return "";
	}

	Fischer_Heun::Fischer_Heun(const std::vector<int>& a) {

		// derived as the most optimal block size
		int b = 0.5*std::log2(a);



	}

	int Fischer_Heun::query([[maybe_unused]] Range range) {
		return 0;
	}
}
