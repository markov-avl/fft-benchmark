cmake_minimum_required(VERSION 3.16)
project(FFTBenchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -lfftw3 -lm -pthread")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
message("SOURCES=${SOURCES}")

add_executable(BenchmarkRadix2 benchmarks/benchmark_radix2.cpp ${SOURCES})
target_include_directories(BenchmarkRadix2 PUBLIC src)
target_link_libraries(BenchmarkRadix2 PRIVATE fftw3 m)

add_executable(Tests tests/tests.cpp ${SOURCES})
target_include_directories(Tests PUBLIC src)
target_link_libraries(Tests PRIVATE fftw3 m)
