
#include <gloperate/navigation/AbstractMapping.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/input/AbstractEventProvider.h>


namespace gloperate
{


AbstractMapping::AbstractMapping()
: m_painter(nullptr)
{
}

AbstractMapping::~AbstractMapping()
{
    for (AbstractEventProvider * provider : m_providers)
    {
        provider->deregisterMapping(this);
    }
}

bool AbstractMapping::hasPainter() const
{
    return m_painter != nullptr;
}

Painter * AbstractMapping::painter() const
{
    return m_painter;
}

void AbstractMapping::setPainter(Painter * painter)
{
    if (m_painter != painter)
    {
        m_painter = painter;
    }

    initializeTools(); //TODO use signal instead?
}

void AbstractMapping::addProvider(AbstractEventProvider * provider)
{
    m_providers.insert(provider);
    provider->registerMapping(this);
}

void AbstractMapping::removeProvider(AbstractEventProvider * provider)
{
    provider->deregisterMapping(this);
    m_providers.erase(provider);
}

void AbstractMapping::processEvent(AbstractEvent * event)
{
    mapEvent(event);
}


} // namespace gloperate
