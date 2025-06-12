#pragma once

#include <map>
#include <memory>
#include <ranges>

#include "algorithm/IFourierTransformAlgorithm.h"
#include "algorithm/impl/dft/DFT.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_I_BRP_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_BRP_Barrier_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_BRP_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_InPlace_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_R2.h"
#include "algorithm/impl/fft/coprime/GoodThomas_I.h"
#include "algorithm/impl/fft/radix2/Stockham_I_R2.h"
#include "algorithm/impl/fft/radix2/Stockham_R_R2.h"
#include "algorithm/impl/fft/radix4/Stockham_I_R4.h"
#include "algorithm/impl/fftw/FFTWEstimate.h"


static const std::map<std::string, std::shared_ptr<IFourierTransformAlgorithm> > ALGORITHMS = {
    {DFT::NAME, std::make_shared<DFT>()},
    {CooleyTukey_I_BRP_R2::NAME, std::make_shared<CooleyTukey_I_BRP_R2>()},
    {CooleyTukey_R_BRP_Barrier_R2::NAME, std::make_shared<CooleyTukey_R_BRP_Barrier_R2>()},
    {CooleyTukey_R_BRP_R2::NAME, std::make_shared<CooleyTukey_R_BRP_R2>()},
    {CooleyTukey_R_InPlace_R2::NAME, std::make_shared<CooleyTukey_R_InPlace_R2>()},
    {CooleyTukey_R_R2::NAME, std::make_shared<CooleyTukey_R_R2>()},
    {GoodThomas_I::NAME, std::make_shared<GoodThomas_I>()},
    {Stockham_I_R2::NAME, std::make_shared<Stockham_I_R2>()},
    {Stockham_R_R2::NAME, std::make_shared<Stockham_R_R2>()},
    {Stockham_I_R4::NAME, std::make_shared<Stockham_I_R4>()},
    {FFTWEstimate::NAME, std::make_shared<FFTWEstimate>()}
};

inline std::vector<std::string> supported_algorithms() {
    std::vector<std::string> algorithm_names;
    for (const auto &name: ALGORITHMS | std::views::keys) {
        algorithm_names.emplace_back(name);
    }
    return algorithm_names;
}
