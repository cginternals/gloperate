#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>
#include <gloperate/Viewport.h>

namespace gloperate
{

class GLOPERATE_API ViewportCapability : public AbstractViewportCapability
{
public:
    ViewportCapability();
    virtual ~ViewportCapability();

    virtual void setViewport(const Viewport & viewport) override;
    virtual const Viewport & viewport() const override;
    virtual int x() const override;
    virtual int y() const override;
    virtual int width() const override;
    virtual int height() const override;

protected:
    Viewport m_viewport;
};

} // namespace gloperate
