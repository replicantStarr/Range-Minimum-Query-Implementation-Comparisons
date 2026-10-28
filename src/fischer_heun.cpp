#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/common.h"
#include <cmath>
#include <unordered_map>

namespace range_minimum_query {

	// This is an implementation of the Fischer-Heun algorithm to solve RMQ, broken into 2 parts:
	// PRE-PROCESSING: the constructor
	// QUERYING: query
	


	// The constructor is the pre-processing phase of the algorithm.
	// It takes an array (vector in this case) of integers, and this becomes the only queryable data.  
	// To query newer data, a new class must be instantiated.
	//


	// Given a vector and block size, it will return the lowest value per block.
	std::vector<int> create_summary(int block, 
			const std::vector<int>& a) {
		return {};
	}

	std::vector<std::vector<int>> create_sparse_table(const std::vector<std::vector<int>>& summary) {

	}

	Fischer_Heun::Fischer_Heun(const std::vector<int>& a) {

		// derived as the most optimal block size
		int b = 0.5*std::log2(a);



	}

	int Fischer_Heun::query([[maybe_unused]] Range range) {
		return 0;
	}
}
