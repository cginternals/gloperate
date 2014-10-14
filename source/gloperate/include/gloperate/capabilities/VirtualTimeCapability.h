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

    virtual bool enabled() const override;
    virtual void setEnabled(bool enabled) override;
    virtual float time() const override;
    virtual void setLoopDuration(float duration) override;
    virtual void update(float delta) override;


protected:
    void normalizeTime();


protected:
    bool  m_enabled;
    float m_duration;
    float m_time;


};


} // namespace gloperate
