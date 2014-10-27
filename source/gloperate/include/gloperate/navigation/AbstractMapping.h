#pragma once

#include <gloperate/gloperate_api.h>

#include <queue>

namespace gloperate {

class Painter;
class Event;

class GLOPERATE_API AbstractMapping
{
public:
    AbstractMapping();
    virtual ~AbstractMapping();

    void addPainter(Painter * painter);


protected:
    virtual void processEvent(Event & event) = 0;

protected:
    Painter * m_painter;

};

} // namespace gloperate
