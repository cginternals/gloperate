
#pragma once

#include <gloperate/primitives/MyDrawable.h>


namespace gloperate
{


template <typename VectorType>
void MyDrawable::setData(size_t index, const std::vector<VectorType> & data)
{
    buffer(index)->setData(data);
}

template <typename ArrayType, size_t ArraySize>
void MyDrawable::setData(size_t index, const std::array<ArrayType, ArraySize> & data)
{
    buffer(index)->setData(data);
}


} // namespace gloperate


#include <gloperate/primitives/MyDrawable.hpp>
