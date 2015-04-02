
#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/painter/AbstractDevicePixelRatioCapability.h>


namespace gloperate
{


/**
 *  @brief
 *    Default implementation for AbstractDevicePixelRatioCapability
 */
class GLOPERATE_API DevicePixelRatioCapability : public AbstractDevicePixelRatioCapability
{
public:
    DevicePixelRatioCapability();

    // Virtual functions from AbstractDevicePixelRatioCapability
    virtual float devicePixelRatio() const override;
    virtual void setDevicePixelRatio(float ratio) override;

protected:
    float m_ratio;
};


} // namespace gloperate
