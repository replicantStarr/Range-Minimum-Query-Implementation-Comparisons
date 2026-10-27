#include "range_minimum_query/segment_tree.h"
#include <algorithm>
#include <climits>

namespace range_minimum_query {

	Segment_Tree::Segment_Tree(const std::vector<int>& a) {

		//Initialising the size of the segment tree
		int inputSize{ static_cast<int>(a.size()) };
		int leaves{ 1 };

		while (leaves < inputSize) leaves *= 2;

		segmentTree = std::vector<int>(leaves * 2 - 1, INT_MAX);
		
		//Putting 'a' vector values as leaves for the segment tree
		for (size_t i{ 0 }; i < a.size(); ++i) {
			segmentTree[leaves - 1 + i] = a[i];
		}

		//Filling the rest of the vector backward, finding min of two children respectively
		for (int i{ leaves - 2 }; i >= 0; --i) {
			segmentTree[i] = std::min(segmentTree[i * 2 + 1], segmentTree[i * 2 + 2]);
		}
	}

	int Segment_Tree::rangeMin(int pos, int nodeL, int nodeR, int queryL, int queryR) {
		//No overlap in node to query
		if (queryL > nodeR || queryR < nodeL) {
			return INT_MAX;
		}

		//Complete overlap in node to query
		if (queryL <= nodeL && queryR >= nodeR) {
			return segmentTree[pos];
		}

		//Partial overlap in node to query, recursing the function
		int mid{ (nodeL + nodeR) / 2 };
		return std::min(rangeMin(pos * 2 + 1, nodeL, mid, queryL, queryR), rangeMin(pos * 2 + 2, mid + 1, nodeR, queryL, queryR));

	}

	std::vector<int> Segment_Tree::query(const std::vector<Query>& queries) {

		std::vector<int> results;
		for (const Query& q : queries) {
			results.push_back(rangeMin(0, 0, leaves - 1, static_cast<int>(q[0]), static_cast<int>(q[1])));
		}
		return {0};
	}
}
