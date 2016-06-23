
#include <gloperate/pipeline/AbstractInputSlot.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractInputSlot::AbstractInputSlot()
: m_feedback(false)
{
}

AbstractInputSlot::~AbstractInputSlot()
{
    // Get parent stage
    Stage * stage = parentStage();
    if (!stage)
    {
        return;
    }

    // Remove slot from stage
    if (m_slotType == SlotType::Input) {
        stage->removeInput(this);
    }

    if (m_slotType == SlotType::ProxyOutput) {
        stage->removeProxyOutput(this);
    }
}

bool AbstractInputSlot::isConnected() const
{
    return source() != nullptr;
}

bool AbstractInputSlot::isFeedback() const
{
    return m_feedback;
}

void AbstractInputSlot::setFeedback(bool feedback)
{
    m_feedback = feedback;
}

void AbstractInputSlot::initInputSlot(SlotType type, Stage * parent, cppexpose::PropertyOwnership ownership)
{
    m_slotType = type;

    if (parent && m_slotType == SlotType::Input)
    {
        parent->addInput(this, ownership);
    }

    else if (parent && m_slotType == SlotType::ProxyOutput)
    {
        parent->addProxyOutput(this, ownership);
    }
}


} // namespace gloperate
