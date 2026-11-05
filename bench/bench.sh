for run in 1 2 3 4 5; do
    taskset -c 2 ./build/range_minimum_query_bench \
        --benchmark_out=results_run${run}.csv --benchmark_out_format=csv
done
