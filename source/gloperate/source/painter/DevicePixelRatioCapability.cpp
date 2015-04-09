
#include <gloperate/painter/DevicePixelRatioCapability.h>


namespace gloperate
{

DevicePixelRatioCapability::DevicePixelRatioCapability()
:   m_ratio{1.0}
{
}

float DevicePixelRatioCapability::devicePixelRatio() const
{
    return m_ratio;
}

void DevicePixelRatioCapability::setDevicePixelRatio(float ratio)
{
    m_ratio = ratio;

    setChanged(true);
}

} // namespace gloperate
