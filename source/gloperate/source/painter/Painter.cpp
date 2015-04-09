
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

AbstractCapability * Painter::addCapability(std::unique_ptr<AbstractCapability> capability)
{
    m_capabilities.push_back(std::move(capability));
    return m_capabilities.back().get();
}


} // gloperate
