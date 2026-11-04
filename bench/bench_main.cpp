#include <benchmark/benchmark.h> 
#include "helper.h"
#include "range_minimum_query/naive_linear.h" 
#include "range_minimum_query/naive_block.h" 
#include "range_minimum_query/segment_tree.h" 
#include "range_minimum_query/sparse_table.h" 
#include "range_minimum_query/fischer_heun.h" 

using namespace range_minimum_query;

template <typename Imp>
static void BM_Construct(benchmark::State& state, Generator generate) {
	const size_t size = state.range(0);
	const std::vector<int> a = generate(size);

	for (auto _ : state) {
		Imp imp(a);
		benchmark::DoNotOptimize(imp);
	}

#if TRACK_MEMORY
	const double bytes = measure_bytes([&] { return Imp(a); });
	state.counters["bytes"] = benchmark::Counter(bytes, benchmark::Counter::kDefaults, benchmark::Counter::kIs1024);

	state.counters["bytes/elem"] = bytes / size;
#endif 
} 
template <typename Imp> 
static void BM_Query(benchmark::State& state, Query_Generator generate) { 
	const size_t size = state.range(0); 
	const std::vector<int> a = make_random_array(size); 
	const std::vector<Range> queries = generate(size);

	Imp imp(a); 
	size_t i = 0;
	for (auto _ : state) {
		benchmark::DoNotOptimize(imp.query(queries[i]));
		i = (i + 1) & (QUERY_BATCH - 1);
	}
	
	state.SetItemsProcessed(state.iterations());
}

template <typename Imp>
static void BM_Multiple_Queries(benchmark::State& state) {
	const size_t n = state.range(0);
	const size_t q = state.range(1);
	const auto a = make_random_array(n);
	const auto queries = make_random_queries(n);

	for (auto _ : state) {
		Imp imp(a);

		for (size_t i = 0; i < q; ++i) {
			int res = imp.query(queries[i & (QUERY_BATCH - 1)]);
			benchmark::DoNotOptimize(res);
		}
	}
}

static void BM_FH_Different_Block_Sizes(benchmark::State& state) {
	const size_t b = state.range(0);
	static const std::vector<int> a = make_random_array(1 << 20);

	for (auto _ : state) {
		Fischer_Heun fhb = Fischer_Heun_Benchmark::create(a, b);
		benchmark::DoNotOptimize(fhb);
	}

#if TRACK_MEMORY
	const double bytes = measure_bytes([&] { return Fischer_Heun_Benchmark::create(a, b); });
	state.counters["bytes"] = benchmark::Counter(bytes, benchmark::Counter::kDefaults, benchmark::Counter::kIs1024);

	state.counters["bytes/elem"] = bytes / a.size();
#endif
}

REGISTER_ALGORITHM(Naive_Linear, 1 << 24);
REGISTER_ALGORITHM(Naive_Block, 1 << 24);
REGISTER_ALGORITHM(Segment_Tree, 1 << 24);
REGISTER_ALGORITHM(Sparse_Table, 1 << 24);
REGISTER_ALGORITHM(Fischer_Heun, 1 << 24);

BENCHMARK(BM_FH_Different_Block_Sizes)
	->DenseRange(2, 16, 1)
	->ArgName("b")
	->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

