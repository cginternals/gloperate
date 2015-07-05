
#include <gloperate/painter/Painter.h>


namespace gloperate
{


Painter::Painter(const std::string & name, ResourceManager & resourceManager, const std::string & relDataPath)
: Object(name)
, m_resourceManager(resourceManager)
, m_relDataPath(relDataPath)
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


} // namespace gloperate
