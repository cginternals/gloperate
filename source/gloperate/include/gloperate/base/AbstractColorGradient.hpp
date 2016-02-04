#pragma once

#include <gloperate/base/AbstractColorGradient.h>


namespace gloperate
{


template <size_t numPixels>
std::array<unsigned char, numPixels * sizeof(std::uint32_t)> AbstractColorGradient::pixelData() const
{
    std::array<unsigned char, numPixels * sizeof(std::uint32_t)> data;

    fillPixelData(data.data(), numPixels);

    return data;
}


} // namespace gloperate
