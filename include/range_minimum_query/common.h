#pragma once

/*
 * This file exists as a common abstraction for all three implementations.
 *
 * It makes tesing simple -- we only need to write one set of tests.
 */

#include <stdexcept>

namespace range_minimum_query {
	// Simple compile-time alias so there is no overhead.
	using Range = std::array<std::size_t, 2>;

	// Custom exception for invalid queries {a, b} where b <= a.
	class Invalid_Query_Exception : public std::logic_error {
		using std::logic_error::logic_error;
	};
}

