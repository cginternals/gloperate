
#pragma once

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Base class for drawables
*
*    A drawable is any kind of geometry that can be
*    rendered on the GPU. Subclasses should provide
*    a paint method that executes the actual rendering.
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
    virtual void draw() const = 0;

};


} // namespace gloperate
