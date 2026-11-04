#pragma once
#include <vector>
#include <benchmark/benchmark.h>

std::vector<int> make_random_array(size_t n);
std::vector<int> make_ascending_array(size_t n);
void powers_of_four(benchmark::Benchmark* b, int64_t max_n);
void power_of_two_boundaries(benchmark::Benchmark* b);
