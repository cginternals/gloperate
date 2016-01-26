
#pragma once


#include <gloperate/painter/AbstractCapability.h>


namespace gloperate 
{


/**
*  @brief
*    Capability that allows for specifying the output viewport of a painter
*
*    If a painter supports this capability, it can be used to specify the viewport
*    the painter uses when rendering an image.
*/
class GLOPERATE_API AbstractViewportCapability : public AbstractCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractViewportCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractViewportCapability();

    /**
    *  @brief
    *    Get viewport position x
    *
    *  @return
    *    X-position
    */
    virtual int x() const = 0;

    /**
    *  @brief
    *    Get viewport position y
    *
    *  @return
    *    Y-position
    */
    virtual int y() const = 0;

    /**
    *  @brief
    *    Get viewport width
    *
    *  @return
    *    Width
    */
    virtual int width() const = 0;

    /**
    *  @brief
    *    Get viewport height
    *
    *  @return
    *    Height
    */
    virtual int height() const = 0;

    /**
    *  @brief
    *    Set viewport
    *
    *  @param[in] x
    *    X-position
    *  @param[in] y
    *    Y-position
    *  @param[in] width
    *    Width
    *  @param[in] height
    *    Height
    */
    virtual void setViewport(int x, int y, int width, int height) = 0;
};


} // namespace gloperate
