#pragma once

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/painter/Painter.h>
#include <gloperate/primitives/ScreenAlignedQuad.h>

#include <globjects-painters/globjects_painters_api.h>


namespace gloperate
{

class AbstractViewportCapability;

} // namespace gloperate

class GLOBJECTS_PAINTERS_API TexturedQuad : public gloperate::Painter
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

    globjects::ref_ptr<globjects::Texture>                 m_texture;
    globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;


};
