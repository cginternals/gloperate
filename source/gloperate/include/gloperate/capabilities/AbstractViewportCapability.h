/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate {


class Viewport;


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
    *    Get viewport
    *
    *  @return
    *    Viewport
    */
    virtual const Viewport & viewport() const = 0;

    /**
    *  @brief
    *    Set viewport
    *
    *  @param[in] viewport
    *    Viewport
    */
    virtual void setViewport(const Viewport & viewport) = 0;

    /**
    *  @brief
    *    Get viewport position x
    *
    *  @return
    *    X-position
    */
    int x() const;

    /**
    *  @brief
    *    Get viewport position y
    *
    *  @return
    *    Y-position
    */
    int y() const;

    /**
    *  @brief
    *    Get viewport width
    *
    *  @return
    *    Width
    */
    int width() const;

    /**
    *  @brief
    *    Get viewport height
    *
    *  @return
    *    Height
    */
    int height() const;


};


} // namespace gloperate
