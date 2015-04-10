#pragma once

namespace gloperate
{

/**
*  @brief
*    Type of data provided by the render target
*/
enum class RenderTargetType : unsigned int {
    Color,
    Depth,
    Normal,
    Geometry,
    TextureCoord,
    ObjectID
};

} // namespace gloperate
