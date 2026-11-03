#include <benchmark/benchmark.h>
#include "range_minimum_query/naive_linear.h"

static void BM_NAIVE(benchmark::State& state) {
	const size_t size = state.range(0);
	std::vector<int> x;
	x.reserve(size);
	for (int i = 0; i < size; ++i) {
		x.push_back(static_cast<int>(size));
	}
	
	for (auto _ : state) {
		range_minimum_query::Naive_Linear naive(x);
		benchmark::DoNotOptimize(naive);
	}
}

BENCHMARK(BM_NAIVE)->RangeMultiplier(4)->Range(16, 4096);

BENCHMARK_MAIN();
