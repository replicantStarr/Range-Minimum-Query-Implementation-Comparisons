#pragma once

/*
 * This file exists as a common abstraction for all three implementations.
 *
 * It makes testing simple -- we only need to write one set of tests.
 */

#include <vector>
#include <stdexcept>

namespace range_minimum_query {
	// Simple compile-time alias so there is no overhead.
	using Query = std::array<std::size_t, 2>;

	// Used in tests to make sure only a function with these arguments is given.
	using RMQFn = std::vector<int>(std::vector<Query>& queries);

	// Custom exception for invalid queries {a, b} where b <= a.
	class Invalid_Query_Exception : public std::logic_error {
		using std::logic_error::logic_error;
	};
}

