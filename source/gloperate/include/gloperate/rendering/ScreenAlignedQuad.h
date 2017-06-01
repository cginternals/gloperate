
#pragma once

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>

#include <gloperate/rendering/AbstractDrawable.h>
#include <gloperate/rendering/Drawable.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class AbstractStringSource;
}


namespace gloperate
{


/**
*  @brief
*    Screen aligned quad rendering helper
*
*    This class can be used to render a screen aligned quad.
*/
class GLOPERATE_API ScreenAlignedQuad : public AbstractDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @remarks
    *    Creates OpenGL objects, thus, a current context is required.
    */
    ScreenAlignedQuad();

    /**
    *  @brief
    *    Destructor
    *
    *  @remarks
    *    Destroys OpenGL objects, thus, a current context is required.
    */
    virtual ~ScreenAlignedQuad();

    /**
    *  @brief
    *    Get underlying drawable object
    *
    *  @return
    *    Pointer to the drawable object (always valid)
    */
    const Drawable * drawable() const;

    /**
    *  @brief
    *    Draw geometry
    *
    *  @remarks
    *    This function is the main draw entry point, used by the superclass
    *    AbstractDrawable.
    */
    virtual void draw() const override;


public:
    static std::unique_ptr<globjects::AbstractStringSource> vertexShaderSource();
    static std::unique_ptr<globjects::AbstractStringSource> fragmentShaderSource();
    static std::unique_ptr<globjects::AbstractStringSource> fragmentShaderSourceInverted();


protected:
    std::unique_ptr<Drawable>          m_drawable; ///< Underlying drawable object
    std::unique_ptr<globjects::Buffer> m_buffer;   ///< Pointer to the buffer used by m_drawable
};


} // namespace gloperate
