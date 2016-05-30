
#include <gloperate/pipeline/AbstractData.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractData::AbstractData(SlotType type)
: AbstractSlot(type)
{
}

AbstractData::~AbstractData()
{
}


} // namespace gloperate
