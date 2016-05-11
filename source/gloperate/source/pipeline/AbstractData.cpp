
#include <gloperate/pipeline/AbstractData.h>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractData::AbstractData()
: m_required(false)
{
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

void AbstractData::initData(Stage * parent)
{
    if (parent) {
        parent->registerOutput(this);
    }
}


} // namespace gloperate
