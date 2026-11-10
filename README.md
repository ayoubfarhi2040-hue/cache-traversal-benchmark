# cache-traversal-benchmark

Benchmark of row-major vs column-major traversal of an N x N int array,
measuring time per element for array sizes from 1 KB to 64 MB.
Part of the Student Research Challenge 2026/27, Architecture of Computers,
Vistula University. Author: Ayoub Farhi (student ID 77491).

## Files
- cache_bench.c  : run 1, power-of-two sizes (N = 16 to 4096). Output: results.csv
- cache_bench2.c : run 2, 19 sizes incl. non-power-of-two, pinned to logical
                   processor 0, high priority. Output: results2.csv
- results.csv, results2.csv : raw measurements (30 runs per size and order)

## Test machine
Intel Core i7-13700H, Windows 11 Pro 26H2 (build 26300.9818),
GCC 16.2.0 (MinGW-W64), laptop on mains power.

## Build and run (Windows, PowerShell)
    gcc -O1 -std=c11 cache_bench2.c -o cache_bench2
    cmd /c "cache_bench2.exe > results2.csv"

Use -O1 so the compiler does not swap the loop order. For run 1, use
cache_bench.c and write to results.csv.

## CSV columns
N, array_KB, order (row/column), run (0-29), ns_per_element

## Limitations
One machine, one compiler setting, elapsed time only (no hardware counters).
