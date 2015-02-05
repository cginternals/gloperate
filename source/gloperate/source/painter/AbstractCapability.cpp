
#include <gloperate/painter/AbstractCapability.h>


namespace gloperate 
{


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

void AbstractCapability::setChanged(bool changed)
{
    m_changed = changed;

    if (m_changed)
    {
        this->changed();
    }
}


} // namespace gloperate
