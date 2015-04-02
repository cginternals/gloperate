
#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/painter/AbstractCapability.h>


namespace gloperate 
{

/**
 *  @brief
 *    Capability that allows for specifying the device pixel ratio
 */
class GLOPERATE_API AbstractDevicePixelRatioCapability : public AbstractCapability
{
public:
    virtual ~AbstractDevicePixelRatioCapability();

    /**
     *  @brief
     *    Get device pixel ratio
     *
     *  @return
     *    device pixel ratio
     */
    virtual float devicePixelRatio() const = 0;

    /**
     *  @brief
     *    Set device pixel ratio
     *
     *  @param[in] ratio
     *    device pixel ratio
     */
    virtual void setDevicePixelRatio(float ratio) = 0;
};

} // namespace gloperate
