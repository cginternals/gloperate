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
