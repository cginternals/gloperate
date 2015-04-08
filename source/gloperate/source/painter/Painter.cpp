
#include <gloperate/painter/Painter.h>


namespace gloperate
{


Painter::Painter(ResourceManager & resourceManager, const std::string & name)
:   Object(name)
,   m_resourceManager(resourceManager)
{
}

Painter::~Painter() = default;

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
