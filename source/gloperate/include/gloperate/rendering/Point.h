
#pragma once


#include <memory>

#include <globjects/Buffer.h>

#include <gloperate/rendering/Shape.h>
#include <gloperate/rendering/Drawable.h>


namespace gloperate
{


/**
*  @brief
*    Point shape
*/
class GLOPERATE_API Point : public Shape
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] options
    *    Shape options
    */
    Point(cppassist::Flags<ShapeOption> options = ShapeOption::None);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Point();

    // Virtual AbstractDrawable functions
    virtual void draw() const override;


protected:
    std::unique_ptr<Drawable>          m_drawable;  ///< Underlying drawable object
    std::unique_ptr<globjects::Buffer> m_vertices;  ///< Vertex buffer
    std::unique_ptr<globjects::Buffer> m_texCoords; ///< Texture coordinate buffer
};


} // namespace gloperate
