#include <gtest/gtest.h>
#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/sparse_table.h"
#include "range_minimum_query/segment_tree.h"

template <typename T>
struct Range_Minimum_Query_Test: ::testing::Test {};

using Implementations = ::testing::Types<range_minimum_query::Fischer_Heun, range_minimum_query::Sparse_Table, range_minimum_query::Segment_Tree>;

const std::vector<int> generic_array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

TYPED_TEST_SUITE(Range_Minimum_Query_Test, Implementations);

TYPED_TEST(Range_Minimum_Query_Test, AlwaysThrowsWhenArrayIsEmpty) {
	std::vector<int> a;
	TypeParam imp(a);

	ASSERT_THROW(imp.query({{0, 1}}), std::out_of_range);
}

const std::vector<query> out_of_bounds_queries = {
	{0, 12},
	{5, 12},
	{11, 12}
	{12, 13}
};

TYPED_TEST(Range_Minimum_Query_Test, ThrowsExceptionOnOutOfBounds) {
	TypeParam imp(generic_array);

	for (size_t i = 0; i < out_of_bounds_queries; ++i) {
		SCOPED_TRACE(out_of_bounds_queries[i]);
		EXPECT_THROW(imp.query(out_of_bounds_queries[i]), std::out_of_range);
	}
}


const std::vector<Query> invalid_queries = {
	{-2, -1},
	{-1, -2},
	{-1, -1},
	{0, 0},
	{1, 1},
	{1, 0},
	{-1, 1},
	{1, -1}
};

TYPED_TEST(Range_Minimum_Query_Test, ThrowsExceptionOnInvalidQuery) {
	TypeParam imp(generic_array);

	for (size_t i = 0; i < invalid_queries.size(); ++i) {
		SCOPED_TRACE(invalid_queries[i]);
		EXPECT_THROW(imp.query(invalid_queries[i]), std::range_minimum_query::Invalid_Query_Exception);
	}
}

struct Exp_Query {
	Query query;
	int exp;
}

struct Test_Case {
	std::string name;
	std::vector<int> arr;
	std::vector<Exp_Query> queries;
}

std::vector<Test_Case> cases = {
    {"single element", {7},
        {{{0, 1}, 7}}},

    {"single negative", {-3},
        {{{0, 1}, -3}}},

    {"two ascending", {1, 2},
        {{{0, 1}, 1}, {{1, 2}, 2}, {{0, 2}, 1}}},

    {"two descending", {2, 1},
        {{{0, 1}, 2}, {{1, 2}, 1}, {{0, 2}, 1}}},

    {"ascending", {1, 2, 3, 4, 5, 6, 7, 8},
        {{{0, 8}, 1}, {{3, 8}, 4}, {{7, 8}, 8}, {{2, 5}, 3}, {{5, 6}, 6}}},

    {"descending", {8, 7, 6, 5, 4, 3, 2, 1},
        {{{0, 8}, 1}, {{0, 3}, 6}, {{4, 6}, 3}, {{0, 1}, 8}, {{6, 7}, 2}}},

    {"all equal", {4, 4, 4, 4, 4},
        {{{0, 5}, 4}, {{2, 3}, 4}, {{1, 4}, 4}}},

    {"min at left end", {0, 5, 3, 9, 2, 7},
        {{{0, 6}, 0}, {{1, 6}, 2}, {{1, 4}, 3}, {{2, 3}, 3}}},

    {"min at right end", {6, 9, 4, 8, 5, -1},
        {{{0, 6}, -1}, {{0, 5}, 4}, {{3, 5}, 5}, {{5, 6}, -1}}},

    {"valley", {9, 7, 5, 3, 1, 2, 4, 6, 8},
        {{{0, 9}, 1}, {{0, 4}, 3}, {{5, 9}, 2}, {{3, 6}, 1}, {{4, 5}, 1}, {{6, 8}, 4}}},

    {"peak", {1, 3, 5, 7, 9, 6, 4, 2},
        {{{0, 8}, 1}, {{1, 8}, 2}, {{2, 6}, 5}, {{3, 5}, 7}, {{4, 5}, 9}}},

    {"duplicate minimums", {3, 1, 4, 1, 5, 9, 2, 6, 1},
        {{{0, 9}, 1}, {{2, 3}, 4}, {{4, 8}, 2}, {{4, 6}, 5}, {{2, 4}, 1}, {{5, 7}, 2}}},

    {"alternating", {2, 1, 2, 1, 2, 1, 2},
        {{{0, 1}, 2}, {{0, 2}, 1}, {{2, 3}, 2}, {{4, 7}, 1}, {{6, 7}, 2}}},

    {"negatives and zero", {-2, 0, -5, 3, -5, 1, -1},
        {{{0, 7}, -5}, {{0, 2}, -2}, {{3, 4}, 3}, {{5, 7}, -1}, {{3, 5}, -5}, {{1, 2}, 0}}},

    {"int extremes", {INT_MAX, INT_MIN, 0, INT_MAX},
        {{{0, 4}, INT_MIN}, {{0, 1}, INT_MAX}, {{2, 4}, 0}, {{3, 4}, INT_MAX}, {{1, 2}, INT_MIN}}},

    {"non-power-of-two size", {5, 8, 2, 9, 6, 3, 7, 1, 4, 10, 12, 11, 0},
        {{{0, 13}, 0}, {{0, 12}, 1}, {{0, 7}, 2}, {{3, 7}, 3}, {{8, 12}, 4},
         {{9, 12}, 10}, {{4, 6}, 3}, {{10, 12}, 11}, {{3, 4}, 9}, {{1, 9}, 1}}},

    {"power-of-two size", {10, 3, 15, 7, 12, 1, 9, 14, 4, 11, 6, 16, 2, 13, 8, 5},
        {{{0, 16}, 1}, {{0, 8}, 1}, {{8, 16}, 2}, {{0, 4}, 3}, {{4, 8}, 1}, {{6, 8}, 9},
         {{8, 12}, 4}, {{13, 16}, 5}, {{2, 4}, 7}, {{9, 11}, 6}, {{6, 12}, 4}, {{14, 15}, 8}}},
};

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedWithinBounds) {
	for (const auto& test_case: valid_cases) {
		SCOPED_TRACE(test_case.name);
		TypeParam imp(test_case.name);
		EXPECT_EQ(imp.query(test_case.arr), test_case.exp);
	}
}

TYPED_TEST(Range_Minimum_Query_Test, ReturnsExpectedWithMultipleCalls) {
}


