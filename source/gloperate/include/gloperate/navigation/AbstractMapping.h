#pragma once

#include <gloperate/gloperate_api.h>

#include <set>
#include <functional>

namespace gloperate {

class Painter;
class AbstractEvent;
class AbstractEventProvider;

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
