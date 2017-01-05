#include <gloperate/rendering/TransparencyMasksGenerator.h>

#include <cassert>
#include <cstdint>
#include <algorithm>

#include <glm/common.hpp>


namespace gloperate
{


auto TransparencyMasksGenerator::generateDistributions(unsigned int numSamples) -> std::unique_ptr<maskDistributions_t>
{
    return TransparencyMasksGenerator(numSamples).generateDistributions();
}

TransparencyMasksGenerator::TransparencyMasksGenerator(unsigned int numSamples)
:   m_numSamples{numSamples}
{
}

auto TransparencyMasksGenerator::generateDistributions() -> std::unique_ptr<maskDistributions_t>
{
    generateCombinations();

    auto masks = std::unique_ptr<maskDistributions_t>(new maskDistributions_t());

    for (auto i = 0u; i < masks->size(); ++i)
        generateDistributionForAlpha(i, masks->at(i));

    return std::move(masks);
}

void TransparencyMasksGenerator::generateCombinations()
{
    m_combinationMasks = std::vector<std::vector<mask_t>>{};

    for (auto k = 0u; k <= m_numSamples; ++k)
    {
        auto kCombinations = std::vector<mask_t>{};
        generateCombinationsForK(0x00, 0, k, kCombinations);

        std::random_shuffle(kCombinations.begin(), kCombinations.end());
        m_combinationMasks.push_back(kCombinations);
    }
}

void TransparencyMasksGenerator::generateCombinationsForK(
    const std::bitset<8> & combination
,   unsigned char offset
,   unsigned char k
,   std::vector<mask_t> & combinationMasks)
{
    if (k == 0)
    {
        combinationMasks.push_back(static_cast<std::uint8_t>(combination.to_ulong()));
        return;
    }

    for (auto i = offset; i < m_numSamples - (k - 1); ++i)
    {
        auto newCombination = combination;
        newCombination.set(i, true);
        generateCombinationsForK(newCombination, i + 1, k - 1, combinationMasks);
    }
}

void TransparencyMasksGenerator::generateDistributionForAlpha(
    unsigned int alphaIndex,
    maskDistribution_t & masks)
{
    const auto avgNumSamples = m_numSamples * (static_cast<float>(alphaIndex) / (s_alphaRes));
    const auto lowNumSamples = static_cast<std::size_t>(glm::floor(avgNumSamples));
    const auto highNumSamples = static_cast<std::size_t>(lowNumSamples + 1);

    const auto ratio = 1.0f - glm::fract(avgNumSamples);

    const auto lowNumMasks = static_cast<unsigned int>(ratio * s_numMasks);
    const auto highNumMasks = s_numMasks - lowNumMasks;

    auto maskIt = masks.begin();

    copyMasks(lowNumMasks, m_combinationMasks[lowNumSamples], maskIt);
    copyMasks(highNumMasks, m_combinationMasks[highNumSamples], maskIt);

    assert(maskIt == masks.end());

    std::random_shuffle(masks.begin(), masks.end());
}

void TransparencyMasksGenerator::copyMasks(
    unsigned int numMasks,
    const std::vector<mask_t> & fromMasks,
    maskDistribution_t::iterator & toMaskIt)
{
    while (numMasks > 0)
    {
        if (numMasks >= fromMasks.size())
        {
            toMaskIt = std::copy(fromMasks.begin(), fromMasks.end(), toMaskIt);
            numMasks -= static_cast<std::uint32_t>(fromMasks.size());
        }
        else
        {
            toMaskIt = std::copy(fromMasks.begin(), fromMasks.begin() + numMasks, toMaskIt);
            numMasks -= numMasks;
        }
    }

    assert(numMasks == 0);
}


} // namespace gloperate
