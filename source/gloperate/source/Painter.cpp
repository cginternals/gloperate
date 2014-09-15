/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/Painter.h>


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
Painter::Painter(ResourceManager & resourceManager)
: m_resourceManager(resourceManager)
{
}

/**
*  @brief
*    Destructor
*/
Painter::~Painter()
{
	// Destroy capabilities
    for (AbstractCapability * capability : m_capabilities) {
        delete capability;
    }
}

/**
*  @brief
*    Initialize painter
*/
void Painter::initialize()
{
    onInitialize();
}

/**
*  @brief
*    Render a frame
*/
void Painter::paint()
{
    onPaint();
}

/**
*  @brief
*    Add capability to the painter
*/
void Painter::addCapability(AbstractCapability * capability)
{
    m_capabilities.push_back(capability);
}


} // gloperate
