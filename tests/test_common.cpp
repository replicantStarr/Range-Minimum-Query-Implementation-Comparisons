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

	std::vector<int> ans = imp.query({{0, 0}});

	ASSERT_TRUE(ans.empty());
}

TYPED_TEST(Range_Minimum_Query_Test, ThrowsExceptionOnOutOfBounds) {
}

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedWithinBounds) {
}

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedWithMultipleCalls) {
}


