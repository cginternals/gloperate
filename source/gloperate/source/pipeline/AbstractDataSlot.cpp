
#include <gloperate/pipeline/AbstractDataSlot.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractDataSlot::AbstractDataSlot()
{
}

AbstractDataSlot::~AbstractDataSlot()
{
    Stage * stage = parentStage();

    if (!stage)
    {
        if (m_slotType == SlotType::Parameter) {
            stage->removeParameter(this);
        }

        if (m_slotType == SlotType::Output) {
            stage->removeOutput(this);
        }
    }
}

void AbstractDataSlot::invalidate()
{
}

void AbstractDataSlot::initDataSlot(SlotType type, Stage * parent, cppexpose::PropertyOwnership ownership)
{
    m_slotType = type;

    if (parent && m_slotType == SlotType::Parameter)
    {
        parent->addParameter(this, ownership);
    }

    else if (parent && m_slotType == SlotType::Output)
    {
        parent->addOutput(this, ownership);
    }
}


} // namespace gloperate
