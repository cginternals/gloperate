
#include <gloperate/painter/Painter.h>


namespace gloperate
{


Painter::Painter(ResourceManager & resourceManager, const std::string & name)
:   Object(name)
,   m_resourceManager(resourceManager)
{
}

Painter::~Painter()
{
    for (auto & capability : m_capabilities)
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

AbstractCapability * Painter::addCapability(AbstractCapability * capability)
{
    m_capabilities.push_back(capability);

    return capability;
}


} // gloperate
