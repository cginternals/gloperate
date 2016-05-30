
#include <gloperate/pipeline/AbstractSlot.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/PipelineEvent.h>


namespace gloperate
{


AbstractSlot::AbstractSlot(SlotType type)
: m_slotType(type)
, m_required(false)
{
}

AbstractSlot::~AbstractSlot()
{
}

SlotType AbstractSlot::slotType() const
{
    return m_slotType;
}

Stage * AbstractSlot::parentStage() const
{
    return static_cast<Stage *>(parent());
}

std::string AbstractSlot::qualifiedName() const
{
    std::stringstream ss;

    Stage * stage = parentStage();

    if (stage)
    {
        ss << stage->name() << ".";
    }

    ss << name();

    return ss.str();
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

void AbstractSlot::onRequiredChanged()
{
    Stage * stage = parentStage();

    if (stage)
    {
        stage->promotePipelineEvent(
            PipelineEvent(PipelineEvent::RequiredChanged, stage, this)
        );
    }
}


} // namespace gloperate
