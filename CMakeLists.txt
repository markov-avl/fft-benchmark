cmake_minimum_required(VERSION 3.16)
project(FFTBenchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -lfftw3 -lm -pthread -O3 -fopenmp")
add_definitions(-DTEST_INCORRECT)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
message("SOURCES=${SOURCES}")

add_executable(Main main.cpp CLI/CLI11.hpp ${SOURCES})
target_include_directories(Main PUBLIC src)
target_link_libraries(Main PRIVATE fftw3 m)

add_executable(Tests tests.cpp ${SOURCES})
target_include_directories(Tests PUBLIC src)
target_link_libraries(Tests PRIVATE fftw3 m)
