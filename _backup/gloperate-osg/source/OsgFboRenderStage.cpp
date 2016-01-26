
#include <gloperate-osg/OsgFboRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>


namespace gloperate_osg
{


OsgFboRenderStage::OsgFboRenderStage(const std::string & name)
: OsgRenderStage(name)
, m_colorTextureOsg(nullptr)
, m_depthTextureOsg(nullptr)
{
    // Register output slots
    addOutput("colorTexture", m_colorTexture);
    addOutput("depthTexture", m_depthTexture);
}

OsgFboRenderStage::~OsgFboRenderStage()
{
}

void OsgFboRenderStage::handleViewportChanged()
{
    // Update OSG textures and camera FBO
    updateFbo_osg();
}

void OsgFboRenderStage::postOsgRendering()
{
    // Update gloperate textures
    updateFbo();
}

void OsgFboRenderStage::updateFbo()
{
    // Update color texture
    unsigned int colorTextureId = getOsgTextureId(m_colorTextureOsg);
    if (colorTextureId != 0) {
        m_colorTexture = globjects::Texture::fromId(colorTextureId, gl::GL_TEXTURE_2D);
        m_colorTexture.invalidate();
    }

    // Update depth texture
    unsigned int depthTextureId = getOsgTextureId(m_depthTextureOsg);
    if (depthTextureId != 0) {
        m_depthTexture = globjects::Texture::fromId(depthTextureId, gl::GL_TEXTURE_2D);
        m_depthTexture.invalidate();
    }
}


} // namespace gloperate_osg
