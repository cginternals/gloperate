#include <gloperate/capabilities/AbstractCapability.h>


namespace gloperate {


AbstractCapability::AbstractCapability()
: m_changed(true)
{
}

AbstractCapability::~AbstractCapability()
{
}

bool AbstractCapability::hasChanged() const
{
    return m_changed;
}

void AbstractCapability::setChanged(bool _changed)
{
    m_changed = _changed;

    if (m_changed)
    {
        changed();
    }
}


} // namespace gloperate
