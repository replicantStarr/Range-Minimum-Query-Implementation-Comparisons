#include "range_minimum_query/sparse_table.h"
#include <algorithm>

namespace range_minimum_query {

	Sparse_Table::Sparse_Table(const std::vector<int>& a) {

		// stores the size of array
		n = a.size();

		if (n == 0) {
			return;
		}
		
		// logTable[length] = the biggest power p where 2^p fits in length
		// e.g. logTable[5] = 2, because 4 fits in 5 but 8 does not
		logTable.assign(n + 1, 0);
		for (size_t length = 2; length <= n; ++length) {
			logTable[length] = logTable[length / 2] + 1;
		}

		// columns go from power 0 up to logTable[n], so there are logTable[n] + 1 columns
		size_t numColumns = logTable[n] + 1;
		
		// making n rows, with one slot per column
		sparseTable.resize(n);
		for (size_t i = 0; i < n; ++i) {
			sparseTable[i].resize(numColumns);
		}

		// column 0 holds blocks of length 1 which is just the array itself
		for (size_t i = 0; i < n; ++i) {
			sparseTable[i][0] = a[i];
		}
		
		// each block is the min of its left and right half, which are both in the prev column
		for (size_t power = 1; power < numColumns; ++power) {
			size_t block = size_t(1) << power;
			size_t half = block / 2;
			
			// i + block <= n stops the block from running off end of the array
			for (size_t i = 0; i + block <= n; ++i) {
				int leftHalf = sparseTable[i][power - 1];
				int rightHalf = sparseTable[i + half][power - 1];
				sparseTable[i][power] = std::min(leftHalf, rightHalf);
			}
		}
	}

	int Sparse_Table::query(Range range) {
		size_t queryL = range[0];
		size_t queryR = range[1];

		if (queryL > queryR) {
			throw Invalid_Query_Exception("range[0] is greater than range[1]");
		}
		if (queryR >= n) {
			throw std::out_of_range("range[1] is greater than the size of the original array");
		}

		// largest power of 2 that fits inside the range
		size_t rangeLength = queryR - queryL + 1;
		size_t power = logTable[rangeLength];

		// 2 blocks of length 2^power, covering whole range, overlap is possible but doesn't affect minimum
		int leftMin = sparseTable[queryL][power];
		int rightMin = sparseTable[queryR - (size_t(1) << power) + 1][power];
		return std::min(leftMin, rightMin);
	}
}
