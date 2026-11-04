#include "helper.h"
#include <random>
#include <vector>
#include <benchmark/benchmark.h>


std::vector<int> make_random_array(size_t n) {
	std::mt19937_64 random(42);
	std::vector<int> a(n);
	for (auto& i : a) i = static_cast<int>(random());
	return a;
}

std::vector<int> make_ascending_array(size_t n) {
	std::vector<int> a(n);
	for (size_t i = 0; i < n; ++i) a[i] = static_cast<int>(i);
	return a;
}

void powers_of_four(benchmark::Benchmark* b, int64_t max_n) {
	for (int64_t n = 16; n <= max_n; n *= 4) b->Arg(n);
}

void power_of_two_boundaries(benchmark::Benchmark* b) {
	for (int k = 10; k <= 22; k += 4) {
		b->Arg(int64_t(1) << k);
		b->Arg((int64_t(1) << k) + 1);
	}
}
