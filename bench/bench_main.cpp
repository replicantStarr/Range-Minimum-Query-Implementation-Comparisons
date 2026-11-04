#include <benchmark/benchmark.h> 
#include "bench/helper.h"
#include "range_minimum_query/naive_linear.h" 
#include "range_minimum_query/naive_block.h" 
#include "range_minimum_query/segment_tree.h" 
#include "range_minimum_query/sparse_table.h" 
#include "range_minimum_query/fischer_heun.h" 

template <typename Imp>
static void BM_Construct(benchmark::State& state) {
	const size_t size = state.range(0);
	std::vector<int> x;
	x.reserve(size);
	for (int i = 0; i < size; ++i) {
		x.push_back(static_cast<int>(size));
	}
	
	for (auto _ : state) {
		Imp imp(x);
		benchmark::DoNotOptimize(imp);
	}
}

BENCHMARK_TEMPLATE(BM_Construct, range_minimum_query::Naive_Linear)->RangeMultiplier(4)->Range(1 << 4, 1 << 24)->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE(BM_Construct, range_minimum_query::Naive_Block)->RangeMultiplier(4)->Range(1 << 4, 1 << 24)->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE(BM_Construct, range_minimum_query::Segment_Tree)->RangeMultiplier(4)->Range(1 << 4, 1 << 24)->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE(BM_Construct, range_minimum_query::Sparse_Table)->RangeMultiplier(4)->Range(1 << 4, 1 << 24)->Unit(benchmark::kMicrosecond);
BENCHMARK_TEMPLATE(BM_Construct, range_minimum_query::Fischer_Heun)->RangeMultiplier(4)->Range(1 << 4, 1 << 24)->Unit(benchmark::kMicrosecond);

static void BM_Query_Sample_1(benchmark::State& state) {
	const size_t size = state.range(0);
	std::vector<int> x;
}

BENCHMARK_MAIN();
