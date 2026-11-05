# RANGE MINIMUM QUERY IMPLEMENTATIONS
There are five different implementations of range minimum query in here.

# REPORT
The report is at [report/report.pdf](report/report.pdf).

# HOW TO BUILD 
```sh
cmake -S . -B build
cmake --build build -j
```

## BUILD PHASES
1. **Configure** (`cmake -S . -B build`). CMake selects the build type, which defaults to
   `Release`, and writes `build/compile_commands.json`. It also downloads GoogleTest v1.18.0
   and Google Benchmark v1.10.0, so the first configure needs network access.
2. **Compile** (`cmake --build build -j`). This builds the dependencies and the three
   project targets below.
3. **Test discovery**. After `test_common` links, CMake runs it to list its tests, which
   is how `ctest` learns each test name.

| Target | Output | Contents |
| --- | --- | --- |
| `range_minimum_query` | `build/librange_minimum_query.a` | The five implementations in `src/` |
| `test_common` | `build/test_common` | The GoogleTest suite, linked against the library |
| `range_minimum_query_bench` | `build/range_minimum_query_bench` | The benchmarks, linked against the library |

To build one target:

```sh
cmake --build build -j --target test_common
```

## COMPILER FLAGS
The project uses C++23 with compiler extensions turned off, so GCC and Clang get `-std=c++23`.

For GCC and Clang, a Release build uses:

| Target | Flags |
| --- | --- |
| All targets | `-std=c++23 -O3 -DNDEBUG` |
| `range_minimum_query` | Also `-Wall -Wextra -Wpedantic -march=native` |
| `test_common` | No extra flags |
| `range_minimum_query_bench` | Also `-O3 -march=native` |

`-march=native` tunes the code for the CPU doing the build. It is set on the library because
that is the code being measured. Binaries built with it may not run on a different CPU.

MSVC uses `/W4` for warnings on the library in place of the GCC and Clang warning flags.

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

`Naive_Block` stops at size 2^14, while the other implementations run up to 2^24.
It stores an `n` by `n` table of integers, so its memory grows with the square of `n`:

| `Naive_Block` size | Table memory |
| --- | --- |
| 2^10 | 4 MiB |
| 2^12 | 64 MiB |
| 2^14 | 1 GiB |
| 2^16 | 16 GiB |
| 2^18 | 256 GiB |

## REPEATED RUNS WITH BENCH.SH
`bench/bench.sh` runs the full benchmark suite five times and saves each run to its own CSV
file. Repeating the runs shows how much the timings vary. Build in Release first, as shown
above, then run it from the repository root:

```sh
bash bench/bench.sh
```

The script uses paths relative to the current directory, so it will not find the benchmark
executable from anywhere else. A full run takes about 16 minutes on an 8-core desktop, so
expect the script to take about 80 minutes. Leave the machine idle while it runs.

Each run is pinned to CPU core 2 with `taskset`, which stops the process moving between cores
and reduces noise in the timings. `taskset` is Linux only, and the machine needs at least
three cores. Change the `-c 2` in the script to use a different core.

The results are written to the current directory as `results_run1.csv` to `results_run5.csv`,
and a new run overwrites them. The results from earlier runs are kept in `results/`,
so move new files there yourself if you want to keep them:

```sh
mv results_run*.csv results/
```

Each file starts with a block of system information, and the CSV data begins at the line
starting with `name,`.

The script does not turn off the glibc cache, so its memory counters read low at small sizes.
See [ACCURATE MEMORY RESULTS](#accurate-memory-results) for how to collect those separately.

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
`Naive_Block` stops at 2^14.

`BM_Construct` has one variant per input array:

| Variant | Input array |
| --- | --- |
| `random` | Random integers |
| `ascending` | The values `0` to `n - 1` in order |

`BM_Query` runs on a random array and has one variant per query shape:

| Variant | Queries |
| --- | --- |
| `random` | Ranges with both endpoints chosen at random, so lengths are mixed |
| `short_range` | Random ranges with a length between 1 and 8 |
| `long_range` | Random ranges with a length between `n / 2` and `n` |
| `single` | Ranges that cover a single element |

`BM_Multiple_Queries` uses a random array and random queries. It runs every combination
of `n` and `q`, which shows the point where a higher construction cost pays for itself:

| Parameter | Values |
| --- | --- |
| `n` | 2^10, 2^14, 2^18, 2^22 |
| `q` | 1, 100, 10000, 1000000 |

`Naive_Block` only runs the `n` values up to 2^14.

`BM_FH_Different_Block_Sizes` builds `Fischer_Heun` over a random array of size 2^20,
with block sizes `b` from 2 to 16.

## MEMORY TRACKING
`BM_Construct` and `BM_FH_Different_Block_Sizes` also report how much memory one built
structure uses. Each result gains two counters:

| Counter | Meaning |
| --- | --- |
| `bytes` | Heap memory used by the structure, shown with binary suffixes such as `Ki` and `Mi` |
| `bytes/elem` | `bytes` divided by the number of elements in the input array |

```sh
./build/range_minimum_query_bench --benchmark_filter='BM_Construct<.*>/random/n:1024$'
```

```
BM_Construct<Segment_Tree>/random/n:1024    0.698 us    0.696 us    20076 bytes=8.01562Ki bytes/elem=8.01562
```

After the timing loop finishes, the benchmark reads the heap usage, builds the structure
once more, and reads the heap usage again. The difference is the reported value, and
measuring outside the loop keeps it from affecting the timings.

- **Heap only:** the size of the object itself is not counted, only what it allocates.
- **The input array is not always counted:** `Naive_Linear` and `Fischer_Heun` keep a reference
  to it, so its 4 bytes per element are not in their totals. This is why `Naive_Linear` reports 0.
  `Segment_Tree`, `Sparse_Table` and `Naive_Block` store the values themselves, so theirs include them.
- **Allocator overhead is included:** the value is what `malloc` has in use, so it can be
  slightly above the size of the stored data.

### ACCURATE MEMORY RESULTS
By default, the counters read too low at small sizes, and can even read 0. glibc keeps recently
freed small blocks in a cache and still counts them as in use, so reusing one shows no growth.
The error shrinks as `n` grows. Turn the cache off to collect memory results:

```sh
GLIBC_TUNABLES=glibc.malloc.tcache_count=0 ./build/range_minimum_query_bench --benchmark_filter='BM_Construct|BM_FH'
```

Do not use the timings from this run, because the setting can change the speed of `malloc`.

### EXCLUSIVE TO GLIBC
Memory tracking reads the heap usage with `mallinfo2()`, which only exists in glibc 2.33
or newer. The check is in `bench/helper.h`, and sets `TRACK_MEMORY` for the benchmarks.

| C library | Result |
| --- | --- |
| glibc 2.33 or newer | Benchmarks run and report the memory counters |
| Older glibc | Benchmarks run without the memory counters |
| Not glibc, such as musl, macOS or Windows | Benchmarks run without the memory counters |

# THIRD-PARTY LIBRARIES
This project uses the following libraries. They are not stored in this repository, and
each one remains under its own license.

The C++ libraries are downloaded by CMake during the configure phase:

| Library | Version | Used for | License |
| --- | --- | --- | --- |
| [GoogleTest](https://github.com/google/googletest) | v1.18.0 | Unit tests | BSD 3-Clause |
| [Google Benchmark](https://github.com/google/benchmark) | v1.10.0 | Benchmarks | Apache 2.0 |

The Python libraries are used in `analysis/` to plot benchmark results:

| Library | Used for | License |
| --- | --- | --- |
| [Matplotlib](https://matplotlib.org) | Plotting | Matplotlib License |
| [NumPy](https://numpy.org) | Numerical work | BSD 3-Clause |
| [pandas](https://pandas.pydata.org) | Loading and processing results | BSD 3-Clause |

