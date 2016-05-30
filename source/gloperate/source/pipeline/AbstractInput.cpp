
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
    Stage * stage = parentStage();
    if (stage) {
        stage->removeInput(this);
    }
}

void AbstractInput::initInput(Stage * parent, cppexpose::PropertyOwnership ownership)
{
    if (parent)
    {
        parent->addInput(this, ownership);
    }
}


} // namespace gloperate
