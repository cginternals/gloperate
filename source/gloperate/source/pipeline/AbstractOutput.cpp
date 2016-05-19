
#include <gloperate/pipeline/AbstractOutput.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractOutput::AbstractOutput()
: AbstractData(SlotType::Output)
{
}

AbstractOutput::~AbstractOutput()
{
    if (m_owner) {
        m_owner->unregisterOutput(this);
    }
}

void AbstractOutput::initOutput(Stage * owner)
{
    if (owner) {
        m_owner = owner;
        owner->registerOutput(this);
    }
}


} // namespace gloperate
