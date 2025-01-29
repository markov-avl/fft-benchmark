#include <memory>
#include <vector>
#include "algorithm.h"
#include "output.h"
#include "benchmark.h"


int main() {
    // const std::vector<size_t> sizes = {
    //     128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304
    // };
    const std::vector<size_t> sizes = {
        4194304, 8388608, 16777216, 33554432, 67108864
    };

    const std::vector<std::shared_ptr<IFourierTransformAlgorithm> > algorithms = {
        std::make_shared<FFTWEstimate>(),
        std::make_shared<RecursiveRadix2FFT>(),
        std::make_shared<RecursiveThreadedRadix2FFT>(),
        std::make_shared<RecursiveThreadedBarrierRadix2FFT>()
    };
    const std::vector<std::shared_ptr<IOutputCollector> > collectors = {
        std::make_shared<CsvOutputCollector>("../output/radix-2-time.csv"),
        std::make_shared<StdOutputCollector>(algorithms.size() + 1, 15, true)
    };

    CompositeOutputCollector collector(collectors);
    Timer timer;

    run_experiments(sizes, algorithms, &collector, &timer, 10);
}
