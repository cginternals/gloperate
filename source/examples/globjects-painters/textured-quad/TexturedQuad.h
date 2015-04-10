#pragma once

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>


namespace gloperate
{

class AbstractViewportCapability;

} // namespace gloperate

class TexturedQuad : public gloperate::Painter
{
public:
    TexturedQuad(gloperate::ResourceManager & resourceManager);
    virtual ~TexturedQuad();

protected:
    virtual void onInitialize();
    virtual void onPaint();

protected:
    void createAndSetupTexture();
    void createAndSetupGeometry();

protected:
    /* capabilities */
    gloperate::AbstractViewportCapability * m_viewportCapability;

    globjects::ref_ptr<globjects::Texture>           m_texture;
    globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;
};
