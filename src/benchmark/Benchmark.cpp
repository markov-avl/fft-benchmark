#include "Benchmark.h"

#include <map>
#include <memory>

#include "algorithm.h"
#include "Experiment.h"
#include "generation.h"
#include "multiprocessing.h"
#include "output.h"


Benchmark::Benchmark(const std::vector<std::string> &algorithms,
                     const std::vector<size_t> &sizes,
                     const std::string &measurer,
                     const std::string &generator,
                     const size_t runs)
    : algorithms(algorithms),
      sizes(sizes),
      measurer(MEASURERS.at(measurer)),
      generator(GENERATORS.at(generator)),
      runs(runs) {
}

void Benchmark::run(const std::string &output_file, const size_t threads) const {
    set_max_threads(threads > 0 ? threads : get_system_threads());

    const std::vector<std::shared_ptr<IOutputCollector> > collectors = {
        std::make_shared<CsvOutputCollector>(output_file),
        std::make_shared<StdOutputCollector>(15, true)
    };
    const std::shared_ptr<IOutputCollector> collector = std::make_shared<CompositeOutputCollector>(collectors);

    collector->add("Size");
    for (const auto &algorithm: algorithms) {
        collector->add(algorithm);
    }
    collector->newline();

    for (const auto size: sizes) {
        auto *in = new ft_complex[size];
        auto *in_copy = new ft_complex[size];
        generator->fill(in, size);

        collector->add(std::to_string(size));
        for (auto &algorithm: algorithms) {
            FT_ARRAY_COPY(size, in, in_copy);

            Experiment experiment(ALGORITHMS.at(algorithm), measurer, runs, size, in_copy);
            collector->add(experiment.run());
        }

        delete[] in;
        delete[] in_copy;
        collector->newline();
    }
}
