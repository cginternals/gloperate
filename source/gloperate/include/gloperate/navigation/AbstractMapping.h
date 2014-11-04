#pragma once

#include <gloperate/gloperate_api.h>

namespace gloperate {

class Painter;
class AbstractEvent;

class GLOPERATE_API AbstractMapping
{
public:
    AbstractMapping();
    virtual ~AbstractMapping();

    bool hasPainter() const;
    void setPainter(Painter * painter);

    virtual void initializeNavigation() = 0;
    virtual void processEvent(AbstractEvent * event) = 0;

protected:
    Painter * m_painter;

};

} // namespace gloperate
