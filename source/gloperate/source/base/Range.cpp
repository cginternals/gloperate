
#include <gloperate/base/Range.h>


namespace gloperate
{


Range::Range()
: m_minValue(0.0f)
, m_maxValue(0.0f)
{
}

Range::Range(float minValue, float maxValue)
: m_minValue(minValue)
, m_maxValue(maxValue)
{
}

float Range::minimumValue() const
{
    return m_minValue;
}

void Range::setMinimumValue(float minValue)
{
    m_minValue = minValue;
}

float Range::maximumValue() const
{
    return m_maxValue;
}

void Range::setMaximumValue(float maxValue)
{
    m_maxValue = maxValue;
}


} // namespace gloperate
