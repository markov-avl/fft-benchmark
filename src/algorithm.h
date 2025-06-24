#pragma once

#include <map>
#include <memory>
#include <ranges>

#include "algorithm/IFourierTransformAlgorithm.h"
#include "algorithm/impl/dft/DFT.h"
#include "algorithm/impl/fft/any/Bluestein_I_Stockham_R2.h"
#include "algorithm/impl/fft/any/Bluestein_I_Stockham_R4.h"
#include "algorithm/impl/fft/coprime/GoodThomas_I.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_I_Barrier_BRP_Stride_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_I_BRP_Stride_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_BRP_Barrier_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_BRP_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_R2.h"
#include "algorithm/impl/fft/radix2/CooleyTukey_R_Stride_R2.h"
#include "algorithm/impl/fft/radix2/Stockham_I_R2.h"
#include "algorithm/impl/fft/radix2/Stockham_R_R2.h"
#include "algorithm/impl/fft/radix4/CooleyTukey_I_Barrier_DRP_Stride_R4.h"
#include "algorithm/impl/fft/radix4/CooleyTukey_I_DRP_Stride_R4.h"
#include "algorithm/impl/fft/radix4/CooleyTukey_R_DRP_Barrier_R4.h"
#include "algorithm/impl/fft/radix4/CooleyTukey_R_DRP_R4.h"
#include "algorithm/impl/fft/radix4/CooleyTukey_R_R4.h"
#include "algorithm/impl/fft/radix4/CooleyTukey_R_Stride_R4.h"
#include "algorithm/impl/fft/radix4/Stockham_I_R4.h"
#include "algorithm/impl/fft/radix4/Stockham_R_R4.h"
#include "algorithm/impl/fftw/FFTWEstimate.h"
#include "algorithm/utils/operation.h"


static const std::map<std::string, std::shared_ptr<IFourierTransformAlgorithm> > ALGORITHMS = {
    {DFT::NAME, std::make_shared<DFT>()},
    {Bluestein_I_Stockham_R2::NAME, std::make_shared<Bluestein_I_Stockham_R2>()},
    {Bluestein_I_Stockham_R4::NAME, std::make_shared<Bluestein_I_Stockham_R4>()},
    {GoodThomas_I::NAME, std::make_shared<GoodThomas_I>()},
    {CooleyTukey_I_Barrier_BRP_Stride_R2::NAME, std::make_shared<CooleyTukey_I_Barrier_BRP_Stride_R2>()},
    {CooleyTukey_I_BRP_Stride_R2::NAME, std::make_shared<CooleyTukey_I_BRP_Stride_R2>()},
    {CooleyTukey_R_BRP_Barrier_R2::NAME, std::make_shared<CooleyTukey_R_BRP_Barrier_R2>()},
    {CooleyTukey_R_BRP_R2::NAME, std::make_shared<CooleyTukey_R_BRP_R2>()},
    {CooleyTukey_R_R2::NAME, std::make_shared<CooleyTukey_R_R2>()},
    {CooleyTukey_R_Stride_R2::NAME, std::make_shared<CooleyTukey_R_Stride_R2>()},
    {Stockham_I_R2::NAME, std::make_shared<Stockham_I_R2>()},
    {Stockham_R_R2::NAME, std::make_shared<Stockham_R_R2>()},
    {CooleyTukey_I_Barrier_DRP_Stride_R4::NAME, std::make_shared<CooleyTukey_I_Barrier_DRP_Stride_R4>()},
    {CooleyTukey_I_DRP_Stride_R4::NAME, std::make_shared<CooleyTukey_I_DRP_Stride_R4>()},
    {CooleyTukey_R_DRP_Barrier_R4::NAME, std::make_shared<CooleyTukey_R_DRP_Barrier_R4>()},
    {CooleyTukey_R_DRP_R4::NAME, std::make_shared<CooleyTukey_R_DRP_R4>()},
    {CooleyTukey_R_R4::NAME, std::make_shared<CooleyTukey_R_R4>()},
    {CooleyTukey_R_Stride_R4::NAME, std::make_shared<CooleyTukey_R_Stride_R4>()},
    {Stockham_I_R4::NAME, std::make_shared<Stockham_I_R4>()},
    {Stockham_R_R4::NAME, std::make_shared<Stockham_R_R4>()},
    {FFTWEstimate::NAME, std::make_shared<FFTWEstimate>()}
};

inline std::vector<std::string> supported_algorithms() {
    std::vector<std::string> algorithm_names;
    for (const auto &name: ALGORITHMS | std::views::keys) {
        algorithm_names.emplace_back(name);
    }
    return algorithm_names;
}
