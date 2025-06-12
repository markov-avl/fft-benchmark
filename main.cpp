#include <iostream>

#include "CLI/CLI11.hpp"
#include "benchmark.h"
#include "multiprocessing.h"


template<typename T>
std::string to_string(const std::vector<T> &vec, const std::string &delimiter = ", ") {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i + 1 < vec.size()) oss << delimiter;
    }
    return oss.str();
}


int main(const int argc, char *argv[]) {
    CLI::App app{"FFT Benchmarking Tool"};

    std::vector<std::string> algorithms;
    std::vector<size_t> sizes;
    std::string measurer;
    std::string generator;
    std::string output_file;
    size_t runs;
    size_t threads;

    app.add_option("-a,--algorithms", algorithms, "Algorithms to compare")
            ->check(CLI::IsMember(supported_algorithms()))
            ->required();
    app.add_option("-s,--sizes", sizes, "Sizes of the input data (e.g., 1024, 2048, ...)")
            ->check(CLI::PositiveNumber)
            ->required();
    app.add_option("-m,--measurer", measurer, "Measurer to use")
            ->check(CLI::IsMember(supported_measurers()))
            ->default_val("timer");
    app.add_option("-g,--generator", generator, "Input data generator")
            ->check(CLI::IsMember(supported_generators()))
            ->default_val("random");
    app.add_option("-o,--output", output_file, "Output file path")
            ->default_val("output.csv");
    app.add_option("-r,--runs", runs, "Number of runs for each algorithm (default: 10)")
            ->check(CLI::PositiveNumber)
            ->default_val(10);
    app.add_option("-t,--threads", threads, "Maximum number of threads (0 for system)")
            ->check(CLI::NonNegativeNumber)
            ->default_val(0);

    CLI11_PARSE(app, argc, argv);

    std::cout << "Algorithms to compare: " << to_string(algorithms) << std::endl;
    std::cout << "Sizes of the input data: " << to_string(sizes) << std::endl;
    std::cout << "Measurer to use: " << measurer << std::endl;
    std::cout << "Input data generator: " << generator << std::endl;
    std::cout << "Output file path: " << output_file << std::endl;
    std::cout << "Number of runs for each algorithm: " << runs << std::endl;
    std::cout << "Maximum number of threads: " << (threads > 0 ? threads : get_system_threads()) << std::endl;
    std::cout << std::endl;

    const Benchmark benchmark(algorithms, sizes, measurer, generator, runs);
    benchmark.run(output_file, threads);

    return 0;
}
