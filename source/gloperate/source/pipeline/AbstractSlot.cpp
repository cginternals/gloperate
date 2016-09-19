
#include <gloperate/pipeline/AbstractSlot.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Pipeline.h>


namespace gloperate
{


AbstractSlot::AbstractSlot()
: m_slotType(SlotType::Empty)
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


} // namespace gloperate
