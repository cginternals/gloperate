
#include <gloperate/pipeline/AbstractDataSlot.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractDataSlot::AbstractDataSlot(SlotType type)
: AbstractSlot(type)
{
}

AbstractDataSlot::~AbstractDataSlot()
{
}


} // namespace gloperate
