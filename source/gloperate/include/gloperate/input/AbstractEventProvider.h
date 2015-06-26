
#pragma once


#include <list>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class AbstractEvent;
class AbstractMapping;


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
