#include <gloperate/navigation/AbstractMapping.h>


namespace gloperate
{

AbstractMapping::AbstractMapping()
{
}

AbstractMapping::~AbstractMapping()
{
}

bool AbstractMapping::hasPainter() const
{
    return m_painter != nullptr;
}

void AbstractMapping::setPainter(Painter * painter)
{
    if (m_painter != painter)
    {
        m_painter = painter;
    }
}

} // namespace gloperate
