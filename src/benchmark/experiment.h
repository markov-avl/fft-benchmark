#ifndef EXPERIMENT_H
#define EXPERIMENT_H


void run_experiments(const std::vector<size_t> &sizes,
                     const std::vector<std::shared_ptr<IFourierTransformAlgorithm> > &algorithms,
                     IOutputCollector *output,
                     IMeasurer *measurer,
                     size_t runs);


#endif
