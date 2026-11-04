#include "helper.h"
#include <random>
#include <vector>

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
