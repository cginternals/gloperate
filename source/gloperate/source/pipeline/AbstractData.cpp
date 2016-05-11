
#include <gloperate/pipeline/AbstractData.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractData::AbstractData(Stage * parent)
: m_required(false)
{
    if (parent) {
        parent->registerOutput(this);
    }
}

AbstractData::~AbstractData()
{
}

bool AbstractData::required() const
{
    return m_required;
}

void AbstractData::setRequired(bool required)
{
    m_required = required;
}


} // namespace gloperate
