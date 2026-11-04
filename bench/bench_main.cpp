#include <benchmark/benchmark.h> 
#include "helper.h"
#include "range_minimum_query/naive_linear.h" 
#include "range_minimum_query/naive_block.h" 
#include "range_minimum_query/segment_tree.h" 
#include "range_minimum_query/sparse_table.h" 
#include "range_minimum_query/fischer_heun.h" 

using namespace range_minimum_query;

using Generator = std::vector<int> (*)(size_t);
using Query_Generator = std::vector<Range> (*)(size_t);

template <typename Imp>
static void BM_Construct(benchmark::State& state, Generator generate) {
	const size_t size = state.range(0);
	const std::vector<int> a = generate(size);

	for (auto _ : state) {
		Imp imp(a);
		benchmark::DoNotOptimize(imp);
	}
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
		Fisher_Heun_Benchmark fhb(a, b);
		benchmark::DoNotOptimize(fhb);
	}
}

#define REGISTER_BUILD(Alg, max_n)							\
	BENCHMARK_CAPTURE(BM_Construct<Alg>, random, make_random_array)			\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kMicrosecond);				\
	BENCHMARK_CAPTURE(BM_Construct<Alg>, ascending, make_ascending_array)		\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kMicrosecond)				

#define REGISTER_QUERY(Alg, max_n)							\
	BENCHMARK_CAPTURE(BM_Query<Alg>, random, make_random_queries)			\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kNanosecond);				\
	BENCHMARK_CAPTURE(BM_Query<Alg>, short_range, make_short_queries)		\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kNanosecond);				\
	BENCHMARK_CAPTURE(BM_Query<Alg>, long_range, make_long_queries)			\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kNanosecond);				\
	BENCHMARK_CAPTURE(BM_Query<Alg>, single, make_single_queries)			\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kNanosecond)				

#define REGISTER_MULTIPLE_QUERIES(Alg, max_n)						\
	BENCHMARK(BM_Multiple_Queries<Alg>)						\
		->Apply([](benchmark::Benchmark* b) { workload_sizes(b, max_n); })	\
		->ArgNames({"n", "q"})							\
		->Unit(benchmark::kMillisecond)						



/*
REGISTER_BUILD(Naive_Linear, 1 << 24);
REGISTER_BUILD(Fischer_Heun, 1 << 24);

REGISTER_QUERY(Naive_Linear, 1 << 24);
REGISTER_QUERY(Fischer_Heun, 1 << 24);
*/

BENCHMARK_CAPTURE(BM_Construct<Fischer_Heun>, same_blocks, make_same_blocks 

REGISTER_MULTIPLE_QUERIES(Naive_Linear, 1 << 24);

BENCHMARK_MAIN();

