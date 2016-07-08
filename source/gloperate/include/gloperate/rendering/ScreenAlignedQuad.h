
#pragma once

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>
#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/rendering/AbstractDrawable.h>
#include <gloperate/rendering/Drawable.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Screen aligned quad rendering helper
*
*    This class can be used to render a screen aligned quad.
*/
class GLOPERATE_API ScreenAlignedQuad : public globjects::Referenced, gloperate::AbstractDrawable
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
    gloperate::Drawable * drawable() const;

    /**
    *  @brief
    *    Draw geometry
    *
    *  @remarks
    *    This function is the main draw entry point, used by the superclass
    *    AbstractDrawable.
    */
    virtual void draw() const override;


protected:
    globjects::ref_ptr<gloperate::Drawable> m_drawable;
};

}
