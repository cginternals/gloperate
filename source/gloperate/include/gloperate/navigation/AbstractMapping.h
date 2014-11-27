#pragma once

#include <gloperate/gloperate_api.h>

#include <list>
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

    void makeCurrent() const;
    void setMakeCurrent(std::function<void ()> func);

    void doneCurrent() const;
    void setDoneCurrent(std::function<void ()> func);

    virtual void initializeTools() = 0;
    virtual void processEvent(AbstractEvent * event);

protected:
    virtual void mapEvent(AbstractEvent * event) = 0;

protected:
    Painter * m_painter;
    std::list<AbstractEventProvider *> m_providers;
    
    std::function<void ()> m_makeCurrent;
    std::function<void ()> m_doneCurrent;
};

} // namespace gloperate
