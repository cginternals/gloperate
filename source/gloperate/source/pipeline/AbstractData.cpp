
#include <gloperate/pipeline/AbstractData.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractData::AbstractData(SlotType type)
: AbstractSlot(type)
, m_required(false)
{
}

AbstractData::~AbstractData()
{
}

bool AbstractData::required() const
{
    return m_required;
}

void AbstractData::setRequired(bool required)
{
    m_required = required;
}


} // namespace gloperate
