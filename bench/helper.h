#pragma once
#include <vector>
#include <benchmark/benchmark.h>
#include "range_minimum_query/common.h"
#include <cstddef>

#if defined(__GLIBC__) && __GLIBC_PREREQ(2, 33)
	#include <malloc.h>
	#define TRACK_MEMORY 1
#else
	#define TRACK_MEMORY 0
#endif


using namespace range_minimum_query;

#define REGISTER_ALGORITHM(Alg, max_n)							\
	BENCHMARK_CAPTURE(BM_Construct<Alg>, random, make_random_array)			\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kMicrosecond);				\
	BENCHMARK_CAPTURE(BM_Construct<Alg>, ascending, make_ascending_array)		\
		->Apply([](benchmark::Benchmark* b) { powers_of_four(b, max_n); })	\
		->ArgName("n")->Unit(benchmark::kMicrosecond);				\
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
		->ArgName("n")->Unit(benchmark::kNanosecond);				\
	BENCHMARK(BM_Multiple_Queries<Alg>)						\
		->Apply([](benchmark::Benchmark* b) { workload_sizes(b, max_n); })	\
		->ArgNames({"n", "q"})							\
		->Unit(benchmark::kMillisecond)						

using Generator = std::vector<int> (*)(size_t);
using Query_Generator = std::vector<Range> (*)(size_t);

inline std::size_t heap_bytes() {
#if TRACK_MEMORY
	const struct mallinfo2 info = mallinfo2();
	return info.uordblks + info.hblkhd;
#else
	return 0;
#endif
}

template <typename C>
double measure_bytes(C c) {
#if TRACK_MEMORY
	const std::size_t before = heap_bytes();
	auto object = c();
	return static_cast<double>(heap_bytes() - before);
#else
	return 0;
#endif
}

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
