/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/Viewport.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractViewportCapability
*/
class GLOPERATE_API ViewportCapability : public AbstractViewportCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    ViewportCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ViewportCapability();

    /**
    *  @brief
    *    Get viewport
    *
    *  @return
    *    Viewport
    */
    virtual const Viewport & viewport() const override;

    /**
    *  @brief
    *    Set viewport
    *
    *  @param[in] viewport
    *    Viewport
    */
    virtual void setViewport(const Viewport & viewport) override;


protected:
    Viewport m_viewport;	/**< The current viewport */


};


} // namespace gloperate
