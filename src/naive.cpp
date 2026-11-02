#include "range_minimum_query/naive.h"
#include "range_minimum_query/common.h"
#include <iostream>

namespace range_minimum_query {
	Naive::Naive(const std::vector<int>& a) : m(a.size(), std::vector<int>(a.size(), 0)) {
		for (size_t i = 0; i < a.size(); ++i) {
			m[i][i] = a[i];
			for (size_t j = i + 1; j < a.size(); ++j) {
				m[i][j] = std::min(m[i][j - 1], a[j]);
			}
		}
	}

	int Naive::query(Range range) {
		if (range[0] >= range[1]) throw Invalid_Query_Exception("Test");
		if (range[1] > m.size()) throw std::out_of_range("test");
		std::cout << range[0] << " " << range[1] << " " << m.size() << std::endl;
		return m[range[0]][range[1] - 1];
	}
}
