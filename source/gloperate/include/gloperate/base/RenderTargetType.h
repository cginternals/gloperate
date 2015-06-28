
#pragma once


namespace gloperate
{


/**
*  @brief
*    Type of data provided by a render target
*/
enum class RenderTargetType : unsigned int
{
    Color,
    Depth,
    Normal,
    Geometry,
    TextureCoord,
    ObjectID
};


} // namespace gloperate
