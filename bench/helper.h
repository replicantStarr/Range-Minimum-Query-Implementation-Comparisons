#pragma once
#include <vector>
#include <benchmark/benchmark.h>
#include "range_minimum_query/common.h"

using namespace range_minimum_query;

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

using Generator = std::vector<int> (*)(size_t);
using Query_Generator = std::vector<Range> (*)(size_t);

inline constexpr std::size_t QUERY_BATCH = 1 << 16;
std::vector<int> make_random_array(size_t n);
std::vector<int> make_ascending_array(size_t n);
void powers_of_four(benchmark::Benchmark* b, int64_t max_n);
void power_of_two_boundaries(benchmark::Benchmark* b);
std::vector<Range> make_random_queries(size_t n);
std::vector<Range> make_short_queries(size_t n);
std::vector<Range> make_long_queries(size_t n);
std::vector<Range> make_single_queries(size_t n);
void workload_sizes(benchmark::Benchmark* b, int64_t max_n);
std::vector<int> make_same_blocks(size_t n);
