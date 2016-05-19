
#include <gloperate/pipeline/AbstractInput.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractInput::AbstractInput()
: AbstractInputSlot(SlotType::Input)
{
}

AbstractInput::~AbstractInput()
{
    if (m_owner) {
        m_owner->unregisterInput(this);
    }
}

void AbstractInput::initInput(Stage * owner)
{
    if (owner) {
        m_owner = owner;
        owner->registerInput(this);
    }
}


} // namespace gloperate
