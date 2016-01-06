
#include <gloperate/painter/Painter.h>


namespace gloperate
{


Painter::Painter(const std::string & name, ResourceManager & resourceManager, const cpplocate::ModuleInfo & /*moduleInfo*/)
: Object(name)
, m_resourceManager(resourceManager)
, m_backgroundColor(0.0, 0.0, 0.0)
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

glm::vec3 Painter::backgroundColor() const
{
	return m_backgroundColor;
}

void Painter::setBackgroundColor(const glm::vec3 & color)
{
	m_backgroundColor = color;
}

AbstractCapability * Painter::addCapability(AbstractCapability * capability)
{
    m_capabilities.push_back(capability);

    return capability;
}


} // namespace gloperate
