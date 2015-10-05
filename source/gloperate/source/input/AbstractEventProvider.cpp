
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
    // Unregister from mappings

    // Note: removeProvider calls registerMapping in turn, which removes the mapping
    // from m_mappings. Therefore, a for-loop would get into trouble, so we iterate
    // like this until the list is empty.

    while (!m_mappings.empty()) {
        AbstractMapping * mapping = m_mappings.front();
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
