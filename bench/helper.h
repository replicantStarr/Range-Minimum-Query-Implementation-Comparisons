#pragma once
#include <vector>
#include <benchmark/benchmark.h>
#include "range_minimum_query/common.h"

using namespace range_minimum_query;

inline constexpr std::size_t QUERY_BATCH = 1 << 16;
std::vector<int> make_random_array(size_t n);
std::vector<int> make_ascending_array(size_t n);
void powers_of_four(benchmark::Benchmark* b, int64_t max_n);
void power_of_two_boundaries(benchmark::Benchmark* b);
std::vector<Range> make_random_queries(size_t n);
std::vector<Range> make_short_queries(size_t n);
std::vector<Range> make_long_queries(size_t n);
std::vector<Range> make_single_queries(size_t n);
