# RANGE MINIMUM QUERY IMPLEMENTATIONS
There are three different implementations of range minimum query in here.

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
| `range_minimum_query` | Also `-Wall -Wextra -Wpedantic` |
| `test_common` | No extra flags |
| `range_minimum_query_bench` | Also `-O3 -march=native` |

`-march=native` applies only to the benchmark executable. The library holds the code being
measured, and it is compiled without that flag. Binaries built with `-march=native` may not
run on a different CPU.

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
**Do not run this without a filter.** `Naive_Block` stores an `n` by `n` table of integers,
and its benchmarks are registered up to 2^24. It will run out of memory and the kernel will
kill the process. The other implementations run up to 2^24, so a complete run takes a long time.

| `Naive_Block` size | Table memory |
| --- | --- |
| 2^10 | 4 MiB |
| 2^12 | 64 MiB |
| 2^14 | 1 GiB |
| 2^16 | 16 GiB |
| 2^18 | 256 GiB |

Run everything except `Naive_Block`, then run `Naive_Block` on the sizes up to 2^14:

```sh
./build/range_minimum_query_bench --benchmark_filter='-Naive_Block'
./build/range_minimum_query_bench --benchmark_filter='Naive_Block.*n:(16|64|256|1024|4096|16384)(/|$)'
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

# One type of benchmark, for all implementations, at small sizes
./build/range_minimum_query_bench --benchmark_filter='BM_Construct<.*>/.*/n:(16|64|256|1024)$'

# One type of benchmark, for one implementation
./build/range_minimum_query_bench --benchmark_filter='BM_Construct<Segment_Tree>'

# One query type, for one implementation
./build/range_minimum_query_bench --benchmark_filter='BM_Query<Fischer_Heun>/short_range'

# One query type, for all implementations, at small sizes
./build/range_minimum_query_bench --benchmark_filter='BM_Query<.*>/single/n:(16|64|256|1024)$'

# One exact benchmark and size
./build/range_minimum_query_bench --benchmark_filter='BM_Query<Sparse_Table>/random/n:1048576$'

# Everything except the matches (a leading - negates the filter)
./build/range_minimum_query_bench --benchmark_filter='-Naive'
```

Add `--benchmark_list_tests` to list the benchmarks that match without running them.
Check this before running a broad filter, because any filter that reaches `Naive_Block`
at a large size will run out of memory.

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
- **`Naive_Linear` reports 0:** it keeps a reference to the input array and allocates nothing.
- **Allocator overhead is included:** the value is what `malloc` has in use, so it can be
  slightly above the size of the stored data.

### EXCLUSIVE TO GLIBC
Memory tracking reads the heap usage with `mallinfo2()`, which only exists in glibc 2.33
or newer. The check is in `bench/helper.h`, and sets `TRACK_MEMORY` for the benchmarks.

| C library | Result |
| --- | --- |
| glibc 2.33 or newer | Benchmarks run and report the memory counters |
| Older glibc | Benchmarks run without the memory counters |
| Not glibc, such as musl, macOS or Windows | `bench/helper.h` currently fails to compile |

