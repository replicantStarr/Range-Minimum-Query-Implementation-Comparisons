#include "range_minimum_query/naive_block.h"
#include "range_minimum_query/common.h"
#include <iostream>

namespace range_minimum_query {
	Naive_Block::Naive_Block(const std::vector<int>& a) : m(a.size(), std::vector<int>(a.size(), 0)) {
		// Simple construction, just get the min of every possible query combination and store it.
		for (size_t i = 0; i < a.size(); ++i) {
			m[i][i] = a[i];
			for (size_t j = i + 1; j < a.size(); ++j) {
				m[i][j] = std::min(m[i][j - 1], a[j]);
			}
		}
	}

	// Simply find the stored query, runs in constant time
	int Naive_Block::query(Range range) {
		if (range[0] > range[1]) throw Invalid_Query_Exception("Test");
		if (range[1] >= m.size()) throw std::out_of_range("test");
		return m[range[0]][range[1]];
	}
}
