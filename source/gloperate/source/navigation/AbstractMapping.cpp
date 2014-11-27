#include <gloperate/navigation/AbstractMapping.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/input/AbstractEventProvider.h>

namespace gloperate
{

AbstractMapping::AbstractMapping()
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
    m_providers.push_back(provider);
    provider->registerMapping(this);
}

void AbstractMapping::removeProvider(AbstractEventProvider * provider)
{
    provider->deregisterMapping(this);
    m_providers.remove(provider);
}

void AbstractMapping::makeCurrent() const
{
    if (m_makeCurrent && m_doneCurrent)
    {
        m_makeCurrent();
    }
}

void AbstractMapping::setMakeCurrent(std::function<void()> func)
{
    m_makeCurrent = func;
}

void AbstractMapping::doneCurrent() const
{
    if (m_doneCurrent)
    {
        m_doneCurrent();
    }
}

void AbstractMapping::setDoneCurrent(std::function<void()> func)
{
    m_doneCurrent = func;
}

void AbstractMapping::processEvent(AbstractEvent * event)
{
    makeCurrent();
    mapEvent(event);
    doneCurrent();
}

} // namespace gloperate
