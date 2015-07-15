
#include <gloperate/input/AbstractEventProvider.h>

#include <gloperate/input/AbstractEvent.h>
#include <gloperate/navigation/AbstractMapping.h>


namespace gloperate
{


AbstractEventProvider::AbstractEventProvider()
{
}

AbstractEventProvider::~AbstractEventProvider()
{
    for (AbstractMapping * mapping : m_mappings)
    {
        mapping->removeProvider(this);
    }
}

void AbstractEventProvider::registerMapping(AbstractMapping * mapping)
{
    m_mappings.push_back(mapping);
}

void AbstractEventProvider::deregisterMapping(AbstractMapping * mapping)
{
    m_mappings.remove(mapping);
}

void AbstractEventProvider::passEvent(AbstractEvent * event)
{
    for (AbstractMapping * mapping : m_mappings)
    {
        mapping->processEvent(event);
    }
}


} // namespace gloperate
