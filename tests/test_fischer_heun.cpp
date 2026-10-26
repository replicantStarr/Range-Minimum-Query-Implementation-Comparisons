#include <gtest/gtest.h>
#include "range_minimum_query/fischer_heun.h"

TEST(FischerHeun, ReturnEmptyOnEmpty) {
	std::vector<int> a;
	Fischer_Heun fischer_heun(a);

	std::vector<int> ans = fischer_heun.query({0});

	ASSERT_TRUE(ans.empty());
}

TEST(FischerHeun, ReturnEmptyOnOutOfBounds) {
	std::vector<int> a;
	Fischer_Heun fischer_heun(a);

	std::vector<int> ans = fischer_heun.query
}

INSTANTIATE_TEST_SUITE_P(


