# RANGE MINIMUM QUERY IMPLEMENTATIONS
There are three different implementations of range minimum query in here.

# HOW TO BUILD 
```sh
cmake -S . -B build
cmake --build build -j
```

# HOW TO RUN TEST
All commands are run from the repository root, after building.

## ALL TESTS
```sh
ctest --test-dir build --output-on-failure
```

## TESTS PER IMPLEMENTATION
`-R` runs only the tests whose name matches a regular expression. Every test name
contains its implementation, so one implementation can be selected by name:

```sh
ctest --test-dir build --output-on-failure -R Fischer_Heun
ctest --test-dir build --output-on-failure -R Sparse_Table
ctest --test-dir build --output-on-failure -R Segment_Tree
ctest --test-dir build --output-on-failure -R Naive_Linear
ctest --test-dir build --output-on-failure -R Naive_Block
```

The same flag also selects by test case, across all implementations or just one:

```sh
ctest --test-dir build --output-on-failure -R ThrowsExceptionOnInvalidQuery
ctest --test-dir build --output-on-failure -R 'ReturnsExpected.*Sparse_Table'
```

Add `-N` to list the tests that match without running them.

# HOW TO RUN BENCHMARKS
Benchmarks must use a Release build, because a Debug build does not optimise the
library and the timings will not be meaningful. An existing `build` directory keeps
its build type, so set it explicitly:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## ALL BENCHMARKS
This runs every benchmark, including array sizes up to 2^24, so it takes a long time.

```sh
./build/range_minimum_query_bench
```

## SPECIFIC BENCHMARKS
`--benchmark_filter` runs only the benchmarks whose name matches a regular expression.
Keep the quotes, because `<`, `>`, `|` and `$` are special characters in the shell.

Benchmark names follow these patterns:

```
BM_Construct<Implementation>/array_type/n:size
BM_Query<Implementation>/query_type/n:size
BM_Multiple_Queries<Implementation>/n:size/q:query_count
BM_FH_Different_Block_Sizes/b:block_size
```

```sh
# One implementation
./build/range_minimum_query_bench --benchmark_filter='Sparse_Table'

# Two implementations
./build/range_minimum_query_bench --benchmark_filter='Segment_Tree|Sparse_Table'

# One type of benchmark, for all implementations
./build/range_minimum_query_bench --benchmark_filter='BM_Construct'

# One type of benchmark, for one implementation
./build/range_minimum_query_bench --benchmark_filter='BM_Construct<Segment_Tree>'

# One query type, for one implementation
./build/range_minimum_query_bench --benchmark_filter='BM_Query<Fischer_Heun>/short_range'

# One query type, for all implementations
./build/range_minimum_query_bench --benchmark_filter='BM_Query<.*>/single'

# One exact benchmark and size
./build/range_minimum_query_bench --benchmark_filter='BM_Query<Sparse_Table>/random/n:1048576$'

# Everything except the matches (a leading - negates the filter)
./build/range_minimum_query_bench --benchmark_filter='-Naive'
```

Add `--benchmark_list_tests` to list the benchmarks that match without running them.

## TYPES OF BENCHMARKS
The first three types run for all five implementations: `Naive_Linear`, `Naive_Block`,
`Segment_Tree`, `Sparse_Table` and `Fischer_Heun`.

| Benchmark | What it measures | Time unit |
| --- | --- | --- |
| `BM_Construct` | Time to build the structure from an array | microseconds |
| `BM_Query` | Time for one query on a structure that is already built | nanoseconds |
| `BM_Multiple_Queries` | Time to build the structure and then run `q` queries, together | milliseconds |
| `BM_FH_Different_Block_Sizes` | Time to build `Fischer_Heun` for each block size | milliseconds |

`BM_Construct` and `BM_Query` use array sizes from 16 to 2^24, multiplying by 4 each step.

`BM_Construct` has one variant per input array:

| Variant | Input array |
| --- | --- |
| `random` | Random integers |
| `ascending` | The values `0` to `n - 1` in order |

`BM_Query` runs on a random array and has one variant per query shape:

| Variant | Queries |
| --- | --- |
| `random` | Random ranges with a length between `n / 2` and `n` |
| `short_range` | Random ranges with a length between 1 and 8 |
| `long_range` | Random ranges with a length between `n / 2` and `n` |
| `single` | Ranges that cover a single element |

`BM_Multiple_Queries` uses a random array and random queries. It runs every combination
of `n` and `q`, which shows the point where a higher construction cost pays for itself:

| Parameter | Values |
| --- | --- |
| `n` | 2^10, 2^14, 2^18, 2^22 |
| `q` | 1, 100, 10000, 1000000 |

`BM_FH_Different_Block_Sizes` builds `Fischer_Heun` over a random array of size 2^20,
with block sizes `b` from 2 to 16.

