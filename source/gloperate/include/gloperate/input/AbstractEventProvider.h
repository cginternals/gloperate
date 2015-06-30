
#pragma once


#include <list>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class AbstractEvent;
class AbstractMapping;


/**
*  @brief
*    Abstract base class for event providers
*
*    An event provider takes input events from the
*    underlying system and transforms them into
*    events for gloperate, which are used, e.g., for
*    interaction and navigation.
*
*    AbstractEventProvider cannot be used directly,
*    but provides a base for specific implementations
*    for different backends such as Qt or GLFW.
*/
class GLOPERATE_API AbstractEventProvider
{
public:
    AbstractEventProvider();
    virtual ~AbstractEventProvider();

    virtual void registerMapping(AbstractMapping * mapping);
    virtual void deregisterMapping(AbstractMapping * mapping);
    virtual void passEvent(AbstractEvent * event);


protected:
    std::list<AbstractMapping*> m_mappings;
};


} // namespace gloperate
