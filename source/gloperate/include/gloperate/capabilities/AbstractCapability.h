/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate {


/**
*  @brief
*    Base class for painter capabilities
*
*    Capabilities are used by painters to communicate their behaviour and supported interfaces.
*    For example, to receive continous updates and timing information, a painter would register
*    a VirtualTimeCapability. To signal that a painter is able to render into a supplied frame buffer
*    object, it would register a TargetFramebufferCapability. Capabilities are used mainly as a
*    communication object between a painter and the outside world, it is not intended to extend
*    the functionality of a painter.
*/
class GLOPERATE_API AbstractCapability
{


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCapability();


};


} // namespace gloperate
