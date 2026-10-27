#include <gtest/gtest.h>
#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/sparse_table.h"
#include "range_minimum_query/segment_tree.h"

template <typename T>
struct Range_Minimum_Query_Test: ::testing::Test {};

using Implementations = ::testing::Types<range_minimum_query::Fischer_Heun, range_minimum_query::Sparse_Table, range_minimum_query::Segment_Tree>;

TYPED_TEST_SUITE(Range_Minimum_Query_Test, Implementations);

TYPED_TEST(Range_Minimum_Query_Test, ReturnsEmptyOnEmpty) {
	std::vector<int> a;
	TypeParam imp(a);

	ASSERT_THROW(imp.query({{0, 1}}), std::out_of_bounds);
}

TYPED_TEST(Range_Minimum_Query_Test, ThrowsExceptionOnOutOfBounds) {

}

const std::vector<int> generic_array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

const std::vector<Query> invalid_queries = {
	{-2, -1},
	{-1, -2},
	{-1, -1},
	{0, 0},
	{1, 1},
	{1, 0}
};

TYPED_TEST(Range_Minimum_Query_Test, ThrowsExceptionOnInvalidQuery) {
	TypeParam imp(generic_array);

	for (size_t i = 0; i < invalid_queries.size(); ++i) {
		SCOPED_TRACE(invalid_queries[i]);
		EXPECT_THROW(imp.query(invalid_queries[i]), std::range_minimum_query::Invalid_Query_Exception);
	}
}

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedWithinBounds) {
}

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedWithMultipleCalls) {
}


