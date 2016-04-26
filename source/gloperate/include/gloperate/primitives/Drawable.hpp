
#pragma once

#include <gloperate/primitives/Drawable.h>


namespace gloperate
{


template <typename VectorType>
void Drawable::setData(size_t index, const std::vector<VectorType> & data)
{
    buffer(index)->setData(data);
}

template <typename ArrayType, size_t ArraySize>
void Drawable::setData(size_t index, const std::array<ArrayType, ArraySize> & data)
{
    buffer(index)->setData(data);
}


} // namespace gloperate
