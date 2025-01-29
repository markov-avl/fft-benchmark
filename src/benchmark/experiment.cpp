#include <iostream>
#include <vector>
#include <numeric>
#include "algorithm.h"
#include "output.h"
#include "experiment.h"


static double measure_experiment(const size_t n,
                                 const std::shared_ptr<IFourierTransformAlgorithm> &algorithm,
                                 IMeasurer *measurer,
                                 const size_t runs) {
    auto *in = static_cast<ft_complex *>(malloc(sizeof(ft_complex) * n));

    for (size_t i = 0; i < n; ++i) {
        in[i][0] = static_cast<double>(i % 256);
        in[i][1] = 0.0;
    }

    std::vector<double> results;

    for (size_t run = 0; run < runs; ++run) {
        auto *out = static_cast<ft_complex *>(malloc(sizeof(ft_complex) * n));

        algorithm->run(n, in, out, measurer);

        results.push_back(measurer->result());
        free(out);
    }

    free(in);

    if (runs > 2) {
        results.erase(std::min_element(results.begin(), results.end()));
        results.erase(std::max_element(results.begin(), results.end()));
    }

    return std::accumulate(results.begin(), results.end(), 0.0) / static_cast<double>(results.size());
}

void run_experiments(const std::vector<size_t> &sizes,
                     const std::vector<std::shared_ptr<IFourierTransformAlgorithm> > &algorithms,
                     IOutputCollector *output,
                     IMeasurer *measurer,
                     const size_t runs) {
    output->add("Size");
    for (const auto &algorithm: algorithms) {
        output->add(algorithm->id());
    }
    output->newline();

    for (const auto &size: sizes) {
        output->add(std::to_string(size));
        for (auto &algorithm: algorithms) {
            const double avg_result = measure_experiment(size, algorithm, measurer, runs);
            output->add(avg_result);
        }
        output->newline();
    }
}
