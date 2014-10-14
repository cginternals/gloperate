#include <gloperate/Painter.h>


namespace gloperate
{


Painter::Painter(ResourceManager & resourceManager)
: m_resourceManager(resourceManager)
{
}

Painter::~Painter()
{
	// Destroy capabilities
    for (AbstractCapability * capability : m_capabilities) {
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
