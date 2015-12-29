
#pragma once


#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>


namespace gloperate
{
    class AbstractViewportCapability;
    class AbstractTargetFramebufferCapability;
}


class ScreenAligned : public gloperate::Painter
{
public:
    ScreenAligned(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo);
    virtual ~ScreenAligned();


protected:
    virtual void onInitialize();
    virtual void onPaint();


protected:
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    /* Capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;

    /* Data */
    globjects::ref_ptr<globjects::Texture>           m_texture;
    globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;
};
