#include <benchmark/benchmark.h> 
#include "helper.h"
#include "range_minimum_query/naive_linear.h" 
#include "range_minimum_query/naive_block.h" 
#include "range_minimum_query/segment_tree.h" 
#include "range_minimum_query/sparse_table.h" 
#include "range_minimum_query/fischer_heun.h" 

using namespace range_minimum_query;

using Generator = std::vector<int> (*)(size_t);

template <typename Imp>
static void BM_Construct(benchmark::State& state, Generator generate) {
	const size_t size = state.range(0);
	const std::vector<int> a = generate(size);

	for (auto _ : state) {
		Imp imp(a);
		benchmark::DoNotOptimize(imp);
	}
}


#define REGISTER_BUILD(Alg, max_n)								\
	BENCHMARK_CAPTURE(BM_Construct<Alg>, random, make_random_array)				\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kMicrosecond);					\
	BENCHMARK_CAPTURE(BM_Construct<Alg>, ascending, make_ascending_array)			\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kMicrosecond)			

REGISTER_BUILD(Naive_Linear, 1 << 24);
REGISTER_BUILD(Fischer_Heun, 1 << 24);

BENCHMARK_MAIN();
