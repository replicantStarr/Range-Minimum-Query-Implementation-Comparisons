#ifndef RANGE_MINIMUM_QUERY_H
#define RANGE_MINIMUM_QUERY_H

/*
 * This file exists as a common abstraction for all three implementations.
 *
 * It makes tesing simple -- we only need to write one set of tests.
 */

#include <utility>
#include <vector>

namespace range_minimum_query {
	// Simple compile-time alias so there is no overhead.
	using Query = std::pair<std::size_t, std::size_t>;

	// Used in tests to make sure only a function with these arguments is given.
	using RMQFn = std::vector<int>(std::vector<Query>& queries);
}

#endif
