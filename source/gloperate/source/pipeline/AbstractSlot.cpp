
#include <gloperate/pipeline/AbstractSlot.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Pipeline.h>


namespace gloperate
{


AbstractSlot::AbstractSlot()
: m_slotType(SlotType::Unknown)
, m_required(false)
, m_feedback(false)
{
}

AbstractSlot::~AbstractSlot()
{
    // Get parent stage
    Stage * stage = parentStage();
    if (!stage) return;

    // Remove slot from stage
    if (m_slotType == SlotType::Input)
    {
        stage->removeInput(this);
    }
    else if (m_slotType == SlotType::Output)
    {
        stage->removeOutput(this);
    }
}

Stage * AbstractSlot::parentStage() const
{
    return static_cast<Stage *>(parent());
}

std::string AbstractSlot::qualifiedName() const
{
    std::string path = name();

    Stage * stage = parentStage();
    path = stage->name() + "." + path;

    Pipeline * pipeline = stage->parentPipeline();
    while (pipeline)
    {
        path = pipeline->name() + "." + path;

        pipeline = pipeline->parentPipeline();
    }

    return path;
}

bool AbstractSlot::isRequired() const
{
    return m_required;
}

void AbstractSlot::setRequired(bool required)
{
    if (m_required != required)
    {
        m_required = required;

        onRequiredChanged();
    }
}

bool AbstractSlot::isFeedback() const
{
    return m_feedback;
}

void AbstractSlot::setFeedback(bool feedback)
{
    m_feedback = feedback;
}

bool AbstractSlot::isConnected() const
{
    return source() != nullptr;
}

void AbstractSlot::initSlot(SlotType slotType, Stage * parent)
{
    m_slotType = slotType;

    if (parent)
    {
        if (m_slotType == SlotType::Input)
        {
            parent->addInput(this);
        }
        else if (m_slotType == SlotType::Output)
        {
            parent->addOutput(this);
        }
    }
}


} // namespace gloperate
