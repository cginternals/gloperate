
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
    if (m_owner) {
        m_owner->unregisterProxyOutput(this);
    }
}

void AbstractProxyOutput::initProxyOutput(Stage * owner)
{
    if (owner) {
        m_owner = owner;
        owner->registerProxyOutput(this);
    }
}


} // namespace gloperate
