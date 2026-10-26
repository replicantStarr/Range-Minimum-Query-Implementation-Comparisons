#include <gtest/gtest.h>
#include "range_minimum_query/fischer_heun.h"
#include "range_minimum_query/sparse_table.h"
#include "range_minimum_query/segment_tree.h"

template <typename T>
struct Range_Minimum_Query_Test ::testing::test {
protected:
	T imp;
}

using Implementations = ::testing::Types<range_minimum_query::Fischer_Heun, range_minimum_query::Sparse_Table, range_minimum_query::Segment_Tree>;
