#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractVirtualTimeCapability.h>

namespace gloperate
{

class GLOPERATE_API VirtualTimeCapability : public AbstractVirtualTimeCapability
{
public:
    VirtualTimeCapability();
    virtual ~VirtualTimeCapability();

    virtual void update(float delta) override;
    virtual void setLoopDuration(float duration) override;
    virtual float time() const override;

protected:
    float m_duration;
    float m_time;

    void normalizeTime();
};

} // namespace gloperate
