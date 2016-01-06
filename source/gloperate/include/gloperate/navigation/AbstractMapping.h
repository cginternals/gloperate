
#pragma once


#include <set>
#include <functional>

#include <gloperate/gloperate_api.h>


namespace gloperate {


class Painter;
class AbstractEvent;
class AbstractEventProvider;


/**
*  @brief
*    Base class for input mapping
*
*    A mapping is used to control interaction in an application.
*    It maps input events to interaction commands, e.g., to
*    control an interaction technique.
*
*    Mappings are application specific, so instead of using this
*    class directly, create your own subclass and specify the
*    mapping of input events to the interaction techniques you want
*    to use.
*/
class GLOPERATE_API AbstractMapping
{
public:
    AbstractMapping();
    virtual ~AbstractMapping();

    bool hasPainter() const;
    Painter * painter() const;
    void setPainter(Painter * painter);

    void addProvider(AbstractEventProvider * provider);
    void removeProvider(AbstractEventProvider * provider);

    virtual void initializeTools() = 0;
    virtual void processEvent(AbstractEvent * event);


protected:
    virtual void mapEvent(AbstractEvent * event) = 0;


protected:
    Painter * m_painter;
    std::set<AbstractEventProvider *> m_providers;
};


} // namespace gloperate
