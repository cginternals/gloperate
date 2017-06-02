
#pragma once


#include <memory>

#include <globjects/Buffer.h>

#include <gloperate/rendering/Shape.h>
#include <gloperate/rendering/Drawable.h>


namespace gloperate
{


/**
*  @brief
*    Box drawable
*/
class GLOPERATE_API Box : public Shape
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] size
    *    Edge width, height, and depth
    *  @param[in] options
    *    Shape options
    */
    Box(float size = 2.0f, cppassist::Flags<ShapeOption> options = ShapeOption::None);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] width
    *    Edge width
    *  @param[in] height
    *    Edge height
    *  @param[in] depth
    *    Edge depth
    *  @param[in] options
    *    Shape options
    */
    Box(float width, float height, float depth, cppassist::Flags<ShapeOption> options = ShapeOption::None);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Box();

    // Virtual AbstractDrawable functions
    virtual void draw() const override;


protected:
    std::unique_ptr<Drawable>          m_drawable;  ///< Underlying drawable object
    std::unique_ptr<globjects::Buffer> m_vertices;  ///< Vertex buffer
    std::unique_ptr<globjects::Buffer> m_texCoords; ///< Texture coordinate buffer
};


} // namespace gloperate
