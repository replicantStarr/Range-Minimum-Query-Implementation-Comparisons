#include <benchmark/benchmark.h>
#include "range_minimum_query/fischer_heun.h"
#include "helper.h"

using namespace range_minimum_query;

static void BM_FH_Different_Block_Sizes(benchmark::State& state) {
	const size_t b = state.range(0);
	static const std::vector<int> a = make_random_array(1 << 20);

	for (auto _ : state) {
		Fisher_Heun_Benchmark fhb(a, b);
		benchmark::DoNotOptimize(fhb);
	}
}

static void BM_FH_Similar_Blocks(benchmark::State& state) {
	const size_t n = state.range(0);
	const std::vector<int> a = make_same_blocks(n);

	for (auto _ : state) {
		Fischer_Heun fh(a);
		benchmark::DoNotOptimize(fh);
	}
}

BENCHMARK(BM_FH_Different_Block_Sizes)
	->DenseRange(2, 16, 1)
	->ArgName("b")
	->Unit(benchmark::kMillisecond)
