
#include <gloperate/painter/AbstractCapability.h>


namespace gloperate 
{

/**
*  @brief
*    Constructor
*/
AbstractCapability::AbstractCapability()
: m_changed(true)
{
}

/**
*  @brief
*    Destructor
*/
AbstractCapability::~AbstractCapability()
{
}

/**
*  @brief
*    Check if the information of this capability have changed
*/
bool AbstractCapability::hasChanged() const
{
    return m_changed;
}

/**
*  @brief
*    Set if the information of this capability have changed
*/
void AbstractCapability::setChanged(bool _changed)
{
    m_changed = _changed;

    if (m_changed)
    {
        changed();
    }
}

} // namespace gloperate
