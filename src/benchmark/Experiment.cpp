#include "Experiment.h"

#include <numeric>
#include <vector>


Experiment::Experiment(const std::shared_ptr<IFourierTransformAlgorithm> &algorithm,
                       const std::shared_ptr<IMeasurer> &measurer,
                       const size_t runs,
                       const size_t n,
                       ft_complex *in)
    : algorithm(algorithm), measurer(measurer), runs(runs), n(n), in(in) {
}

double Experiment::run() const {
    std::vector<double> results;

    for (size_t r = 0; r < runs; ++r) {
        auto *out = new ft_complex[n];

        algorithm->run(n, in, out, measurer.get());
        results.push_back(measurer->result());

        delete[] out;
    }

    if (runs > 2) {
        results.erase(std::min_element(results.begin(), results.end()));
        results.erase(std::max_element(results.begin(), results.end()));
    }

    return std::accumulate(results.begin(), results.end(), 0.0) / static_cast<double>(results.size());
}
