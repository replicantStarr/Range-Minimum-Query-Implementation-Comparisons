# RANGE MINIMUM QUERY IMPLEMENTATIONS
There are three different implementations of range minimum query in here.

# HOW TO BUILD 
```sh
cmake -S . -B build
cmake --build build -j
```

# HOW TO RUN TEST
## ALL TESTS
```sh
ctest --test-dir build --output-on-failure
```

## INDIVIDUAL ALGORITHMS
```sh
./build/test_${algorithm}
```

# HOW TO RUN BENCHMARKS
```sh
./build/range_minimum_query_bench
```

