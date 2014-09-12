/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/Viewport.h>


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
AbstractViewportCapability::AbstractViewportCapability()
{
}

/**
*  @brief
*    Destructor
*/
AbstractViewportCapability::~AbstractViewportCapability()
{
}

/**
*  @brief
*    Get viewport position x
*/
int AbstractViewportCapability::x() const
{
    return viewport().x();
}

/**
*  @brief
*    Get viewport position y
*/
int AbstractViewportCapability::y() const
{
    return viewport().y();
}

/**
*  @brief
*    Get viewport width
*/
int AbstractViewportCapability::width() const
{
    return viewport().width();
}

/**
*  @brief
*    Get viewport height
*/
int AbstractViewportCapability::height() const
{
    return viewport().height();
}


} // namespace gloperate
