#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <vector>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class GLOPERATE_API TransparencyMasksGenerator
{
public:
    static const auto s_alphaRes = 256u;
    static const auto s_numMasks = 1024u;

    using mask_t = unsigned char;
    using maskDistribution_t = std::array<mask_t, s_numMasks>;

    using maskDistributions_t = std::array<maskDistribution_t, s_alphaRes>;

public:
    static std::unique_ptr<maskDistributions_t> generateDistributions(unsigned int numSamples);

public:
    TransparencyMasksGenerator(unsigned int numSamples);

    std::unique_ptr<maskDistributions_t> generateDistributions();

protected:
    void generateCombinations();

    void generateCombinationsForK(
        const std::bitset<8> & combination,
        unsigned char offset,
        unsigned char k,
        std::vector<mask_t> & combinationMasks);

    void generateDistributionForAlpha(
        unsigned int alphaIndex,
        maskDistribution_t & masks);

    void copyMasks(
        unsigned int numMasks,
        const std::vector<mask_t> & fromMasks,
        maskDistribution_t::iterator & toMaskIt);

private:
    const unsigned int m_numSamples;
    std::vector<std::vector<mask_t>> m_combinationMasks;
};


} // namespace gloperate
