#pragma once


#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractViewportCapability.h>


namespace gloperate
{


class GLOPERATE_API ViewportCapability : public AbstractViewportCapability
{


public:
    ViewportCapability();
    virtual ~ViewportCapability();

    virtual int x() const override;
    virtual int y() const override;
    virtual int width() const override;
    virtual int height() const override;
    virtual void setViewport(int x, int y, int width, int height) override;


protected:
    int m_x;
    int m_y;
    int m_width;
    int m_height;


};


} // namespace gloperate
