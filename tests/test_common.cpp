#include <gtest/gtest.h>
#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/sparse_table.h"
#include "range_minimum_query/segment_tree.h"
#include "range_minimum_query/linear.h"
#include "range_minimum_query/naive.h"
#include "common.h"

TYPED_TEST_SUITE(Range_Minimum_Query_Test, Implementations);

TYPED_TEST(Range_Minimum_Query_Test, AlwaysThrowsWhenArrayIsEmpty) {
	std::vector<int> a;
	TypeParam imp(a);

	ASSERT_THROW(imp.query({0, 1}), std::out_of_range);
}

TYPED_TEST(Range_Minimum_Query_Test, ThrowsExceptionOnOutOfBounds) {
	TypeParam imp(generic_array);

	for (size_t i = 0; i < out_of_bounds_ranges.size(); ++i) {
		SCOPED_TRACE("Out of Bound " + std::to_string(i));
		EXPECT_THROW(imp.query(out_of_bounds_ranges[i]), std::out_of_range);
	}
}

TYPED_TEST(Range_Minimum_Query_Test, ThrowsExceptionOnInvalidQuery) {
	TypeParam imp(generic_array);

	for (size_t i = 0; i < invalid_ranges.size(); ++i) {
		SCOPED_TRACE("Invalid Query " + std::to_string(i));
		EXPECT_THROW(imp.query(invalid_ranges[i]), range_minimum_query::Invalid_Query_Exception);
	}
}

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedValueWithValidQuery) {
	for (const auto& test_case: valid_cases) {
		SCOPED_TRACE(test_case.name);

		for (size_t i = 0; i < test_case.ranges.size(); ++i) {
			TypeParam imp(test_case.arr);
			EXPECT_EQ(imp.query(test_case.ranges[i]), test_case.exp[i]);
		}
	}
}

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedValuesWithSequentialValidQueries) {
	for (const auto& test_case: valid_cases) {
		SCOPED_TRACE("Individual " + test_case.name);
		TypeParam imp(test_case.arr);
		for (size_t i = 0; i < test_case.ranges.size(); ++i) {
			EXPECT_EQ(imp.query(test_case.ranges[i]), test_case.exp[i]);
		}
	}
}


