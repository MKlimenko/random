# random

A small library for random numbers generation and statistics measurement. 

Benchmark, comparing to the STL versions:

```
06/02/18 23:31:49
Running G:\Visual Studio\random\make\vs141\x64\Release\benchmark.exe
Run on (4 X 2806 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 262K (x4)
  L3 Unified 8388K (x1)
```
| Benchmark                | Time   | CPU     | Iterations |
| :----------------------- | -----: | ------: | ---------: |
| uniform_STL_mt19937_real |  67 ns |   64 ns |  11217877  |
| uniform_STL_lce_real     |  70 ns |   65 ns |  11217877  |
| uniform_random_real      |   4 ns |    3 ns | 213673844  |
| uniform_STL_mt19937_int  |   9 ns |    9 ns |  89743014  |
| uniform_STL_lce_int      |  16 ns |   16 ns |  56089384  |
| uniform_random_int       |   4 ns |    3 ns | 203961397  |
| normal_STL_mt19937       |  95 ns |   88 ns |   7478585  |
| normal_STL_lce           | 135 ns |  131 ns |   5608938  |
| normal_random            |  38 ns |   36 ns |  18696461  |