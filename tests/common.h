#pragma once

#include <limits.h>

template <typename T>
struct Range_Minimum_Query_Test: ::testing::Test {};

using Implementations = ::testing::Types<range_minimum_query::Fischer_Heun, range_minimum_query::Sparse_Table, range_minimum_query::Segment_Tree>;

inline const std::vector<int> generic_array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

inline const std::vector<range_minimum_query::Query> out_of_bounds_queries = {
	{0, 12},
	{5, 12},
	{11, 12},
	{12, 13}
};

inline const std::vector<range_minimum_query::Query> invalid_queries = {
	{0, 0},
	{1, 1},
	{1, 0},
};

struct Test_Case {
	std::string name;
	std::vector<int> arr;
	std::vector<range_minimum_query::Query> queries;
	std::vector<int> exp;
};

inline const std::vector<Test_Case> valid_cases = {
    {"single element", {7},
        {{0, 1}},
        {7}},

    {"single negative", {-3},
        {{0, 1}},
        {-3}},

    {"two ascending", {1, 2},
        {{0, 1}, {1, 2}, {0, 2}},
        {1, 2, 1}},

    {"two descending", {2, 1},
        {{0, 1}, {1, 2}, {0, 2}},
        {2, 1, 1}},

    {"ascending", {1, 2, 3, 4, 5, 6, 7, 8},
        {{0, 8}, {3, 8}, {7, 8}, {2, 5}, {5, 6}},
        {1, 4, 8, 3, 6}},

    {"descending", {8, 7, 6, 5, 4, 3, 2, 1},
        {{0, 8}, {0, 3}, {4, 6}, {0, 1}, {6, 7}},
        {1, 6, 3, 8, 2}},

    {"all equal", {4, 4, 4, 4, 4},
        {{0, 5}, {2, 3}, {1, 4}},
        {4, 4, 4}},

    {"min at left end", {0, 5, 3, 9, 2, 7},
        {{0, 6}, {1, 6}, {1, 4}, {2, 3}},
        {0, 2, 3, 3}},

    {"min at right end", {6, 9, 4, 8, 5, -1},
        {{0, 6}, {0, 5}, {3, 5}, {5, 6}},
        {-1, 4, 5, -1}},

    {"valley", {9, 7, 5, 3, 1, 2, 4, 6, 8},
        {{0, 9}, {0, 4}, {5, 9}, {3, 6}, {4, 5}, {6, 8}},
        {1, 3, 2, 1, 1, 4}},

    {"peak", {1, 3, 5, 7, 9, 6, 4, 2},
        {{0, 8}, {1, 8}, {2, 6}, {3, 5}, {4, 5}},
        {1, 2, 5, 7, 9}},

    {"duplicate minimums", {3, 1, 4, 1, 5, 9, 2, 6, 1},
        {{0, 9}, {2, 3}, {4, 8}, {4, 6}, {2, 4}, {5, 7}},
        {1, 4, 2, 5, 1, 2}},

    {"alternating", {2, 1, 2, 1, 2, 1, 2},
        {{0, 1}, {0, 2}, {2, 3}, {4, 7}, {6, 7}},
        {2, 1, 2, 1, 2}},

    {"negatives and zero", {-2, 0, -5, 3, -5, 1, -1},
        {{0, 7}, {0, 2}, {3, 4}, {5, 7}, {3, 5}, {1, 2}},
        {-5, -2, 3, -1, -5, 0}},

    {"int extremes", {INT_MAX, INT_MIN, 0, INT_MAX},
        {{0, 4}, {0, 1}, {2, 4}, {3, 4}, {1, 2}},
        {INT_MIN, INT_MAX, 0, INT_MAX, INT_MIN}},

    {"non-power-of-two size", {5, 8, 2, 9, 6, 3, 7, 1, 4, 10, 12, 11, 0},
        {{0, 13}, {0, 12}, {0, 7}, {3, 7}, {8, 12},
         {9, 12}, {4, 6}, {10, 12}, {3, 4}, {1, 9}},
        {0, 1, 2, 3, 4, 10, 3, 11, 9, 1}},

    {"power-of-two size", {10, 3, 15, 7, 12, 1, 9, 14, 4, 11, 6, 16, 2, 13, 8, 5},
        {{0, 16}, {0, 8}, {8, 16}, {0, 4}, {4, 8}, {6, 8},
         {8, 12}, {13, 16}, {2, 4}, {9, 11}, {6, 12}, {14, 15}},
        {1, 1, 2, 3, 1, 9, 4, 5, 7, 6, 4, 8}},
};
