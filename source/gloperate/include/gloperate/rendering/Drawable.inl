
#pragma once


namespace gloperate
{


template <typename VectorType>
void Drawable::setData(size_t index, const std::vector<VectorType> & data, gl::GLenum usage)
{
    buffer(index)->setData(data, usage);
}

template <typename ArrayType, size_t ArraySize>
void Drawable::setData(size_t index, const std::array<ArrayType, ArraySize> & data, gl::GLenum usage)
{
    buffer(index)->setData(data, usage);
}


} // namespace gloperate
