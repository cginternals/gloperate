
#include <gloperate/pipeline/AbstractParameter.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractParameter::AbstractParameter()
: AbstractData(SlotType::Parameter)
{
}

AbstractParameter::~AbstractParameter()
{
    if (m_owner) {
        m_owner->unregisterParameter(this);
    }
}

void AbstractParameter::initParameter(Stage * owner)
{
    if (owner) {
        m_owner = owner;
        owner->registerParameter(this);
    }
}


} // namespace gloperate
