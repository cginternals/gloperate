/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>

#include <gloperate/painter/AbstractCapability.h>
#include <gloperate/base/RenderTarget.h>


namespace gloperate {

    enum class RenderTargetType {
        Color,
        Depth,
        Normal,
        Geometry,
        ObjectID
    };


/**
*  @brief
*    
*/
class GLOPERATE_API AbstractTypedRenderTargetCapability : public AbstractCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractTypedRenderTargetCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractTypedRenderTargetCapability();

    virtual const RenderTarget & renderTarget(RenderTargetType type) = 0;
    virtual bool hasRenderTarget(RenderTargetType type) = 0;

protected:

};


} // namespace gloperate
