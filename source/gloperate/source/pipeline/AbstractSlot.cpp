
#include <gloperate/pipeline/AbstractSlot.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/PipelineEvent.h>


namespace gloperate
{


AbstractSlot::AbstractSlot(SlotType type)
: m_slotType(type)
, m_owner(nullptr)
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

Stage * AbstractSlot::owner() const
{
    return m_owner;
}

std::string AbstractSlot::qualifiedName() const
{
    std::stringstream ss;

    if (m_owner)
    {
        ss << m_owner->name() << ".";
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
    m_owner->promotePipelineEvent(
        PipelineEvent(PipelineEvent::RequiredChanged, m_owner, this)
    );
}


} // namespace gloperate
