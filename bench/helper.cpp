#include "helper.h"
#include <random>
#include <vector>
#include <benchmark/benchmark.h>
#include "range_minimum_query/common.h"

using namespace range_minimum_query; 

std::vector<int> make_random_array(size_t n) {
	std::mt19937_64 random(42);
	std::vector<int> a(n);
	for (auto& i : a) i = static_cast<int>(random());
	return a;
}

std::vector<int> make_ascending_array(size_t n) {
	std::vector<int> a(n);
	for (size_t i = 0; i < n; ++i) a[i] = static_cast<int>(i);
	return a;
}

void powers_of_four(benchmark::Benchmark* b, int64_t max_n) {
	for (int64_t n = 16; n <= max_n; n *= 4) b->Arg(n);
}

void power_of_two_boundaries(benchmark::Benchmark* b) {
	for (int k = 10; k <= 22; k += 4) {
		b->Arg(int64_t(1) << k);
		b->Arg((int64_t(1) << k) + 1);
	}
}

std::vector<Range> make_random_queries(size_t n) {
	std::mt19937_64 random(7);
	std::vector<Range> ranges(QUERY_BATCH);

	for (auto& range: ranges) {
		size_t len = n / 2 + random() % (n - n / 2);
		size_t l = random() % (n - len + 1);
		range = {l, l + len - 1};
	}

	return ranges;
}

std::vector<Range> make_short_queries(size_t n) {
	std::mt19937_64 random(7);
	std::vector<Range> ranges(QUERY_BATCH);
	
	for (auto& range: ranges) {
		size_t len = std::min<size_t>(1 + random() % 8, n);
		size_t l = random() % (n - len + 1);
		range = {l, l + len - 1};
	}

	return ranges;
}

std::vector<Range> make_long_queries(size_t n) {
	std::mt19937_64 random(7);
	std::vector<Range> ranges(QUERY_BATCH);
	
	for (auto& range: ranges) {
		size_t len = n / 2 + random() % (n - n / 2);
		size_t l = random() % (n - len + 1);
		range = {l, l + len - 1};
	}

	return ranges;
}

std::vector<Range> make_single_queries(size_t n) {
	std::mt19937_64 random(7);
	std::vector<Range> ranges(QUERY_BATCH);
	
	for (auto& range: ranges) {
		size_t i = random() % n;
		range = {i, i};
	}

	return ranges;
}
