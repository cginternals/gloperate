
#include <gloperate/pipeline/AbstractProxyOutput.h>

#include <gloperate/pipeline/Pipeline.h>


namespace gloperate
{


AbstractProxyOutput::AbstractProxyOutput()
: AbstractInputSlot(SlotType::ProxyOutput)
{
}

AbstractProxyOutput::~AbstractProxyOutput()
{
    Stage * stage = parentStage();
    if (stage) {
        stage->removeProxyOutput(this);
    }
}

void AbstractProxyOutput::initProxyOutput(Stage * parent, cppexpose::PropertyOwnership ownership)
{
    if (parent)
    {
        parent->addProxyOutput(this, ownership);
    }
}


} // namespace gloperate
