
#include <gloperate/pipeline/AbstractParameter.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractParameter::AbstractParameter()
: AbstractDataSlot(SlotType::Parameter)
{
}

AbstractParameter::~AbstractParameter()
{
    Stage * stage = parentStage();
    if (stage) {
        stage->removeParameter(this);
    }
}

void AbstractParameter::initParameter(Stage * parent, cppexpose::PropertyOwnership ownership)
{
    if (parent)
    {
        parent->addParameter(this, ownership);
    }
}


} // namespace gloperate
