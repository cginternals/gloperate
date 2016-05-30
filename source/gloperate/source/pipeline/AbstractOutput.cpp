
#include <gloperate/pipeline/AbstractOutput.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractOutput::AbstractOutput()
: AbstractDataSlot(SlotType::Output)
{
}

AbstractOutput::~AbstractOutput()
{
    Stage * stage = parentStage();
    if (stage) {
        stage->removeOutput(this);
    }
}

void AbstractOutput::initOutput(Stage * parent, cppexpose::PropertyOwnership ownership)
{
    if (parent)
    {
        parent->addOutput(this, ownership);
    }
}


} // namespace gloperate
