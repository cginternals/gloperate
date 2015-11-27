
#pragma once


#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class VertexArray;
}


namespace gloperate
{


/**
*  @brief
*    Base class for drawables
*
*    A drawable is any kind of geometry that can be
*    rendered on the GPU. It stores its geometry
*    as a vertex array object and provides a paint
*    method that executes the actual rendering.
*/
class GLOPERATE_API AbstractDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractDrawable();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractDrawable();

    /**
    *  @brief
    *    Draw geometry
    */
    virtual void draw() = 0;

};


} // namespace gloperate
