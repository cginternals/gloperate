/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/Painter.h>


namespace gloperate
{


Painter::Painter()
{
}

Painter::~Painter()
{
    for (AbstractCapability * capability : m_capabilities)
    {
        delete capability;
    }
}

void Painter::initialize()
{
    onInitialize();
}

void Painter::paint()
{
    onPaint();
}

void Painter::addCapability(AbstractCapability * capability)
{
    m_capabilities.push_back(capability);
}


} // gloperate
