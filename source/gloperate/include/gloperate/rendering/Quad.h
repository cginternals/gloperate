
#pragma once


#include <memory>

#include <globjects/Buffer.h>

#include <gloperate/rendering/Shape.h>
#include <gloperate/rendering/Drawable.h>


namespace gloperate
{


/**
*  @brief
*    Quad shape
*/
class GLOPERATE_API Quad : public Shape
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] size
    *    Edge width and height
    *  @param[in] options
    *    Shape options
    */
    Quad(float size = 2.0f, cppassist::Flags<ShapeOption> options = ShapeOption::None);

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] width
    *    Edge width
    *  @param[in] height
    *    Edge height
    *  @param[in] options
    *    Shape options
    */
    Quad(float width, float height, cppassist::Flags<ShapeOption> options = ShapeOption::None);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Quad();

    // Virtual AbstractDrawable functions
    virtual void draw() const override;


protected:
    std::unique_ptr<Drawable>          m_drawable;  ///< Underlying drawable object
    std::unique_ptr<globjects::Buffer> m_vertices;  ///< Vertex buffer
    std::unique_ptr<globjects::Buffer> m_texCoords; ///< Texture coordinate buffer
};


} // namespace gloperate
