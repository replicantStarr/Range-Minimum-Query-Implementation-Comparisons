#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/common.h"

namespace range_minimum_query {

	// This is an implementation of the Fischer-Heun algorithm to solve RMQ, broken into 2 parts:
	// PRE-PROCESSING: the constructor
	// QUERYING: query
	


	// The constructor is the pre-processing phase of the algorithm.
	// It takes an array (vector in this case) of integers, and this becomes the only queryable data.  
	// To query newer data, a new class must be instantiated.
	//
	Fischer_Heun::Fischer_Heun([[maybe_unused]] const std::vector<int>& a) {

	}

	int Fischer_Heun::query([[maybe_unused]] Range range) {
		return 0;
	}
}
