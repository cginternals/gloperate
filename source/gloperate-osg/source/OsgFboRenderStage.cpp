#include <gloperate-osg/OsgFboRenderStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>


namespace gloperate_osg
{


/**
*  @brief
*    Constructor
*/
OsgFboRenderStage::OsgFboRenderStage(const std::string & name)
: OsgRenderStage(name)
, m_colorTextureOsg(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
OsgFboRenderStage::~OsgFboRenderStage()
{
}

void OsgFboRenderStage::handleViewportChanged()
{
    // Update OSG textures and camera FBO
    updateFbo_osg();

    // Update gloperate textures
    updateFbo();
}

/**
*  @brief
*    Called to setup textures for use with gloperate pipeline
*/
void OsgFboRenderStage::updateFbo()
{
    // Update color texture
    unsigned int colorTextureId = getOsgTextureId(m_colorTextureOsg);
    if (colorTextureId != 0) {
        m_colorTexture = globjects::Texture::fromId(colorTextureId, gl::GL_TEXTURE_2D);
        m_colorTexture.invalidate();
    }
}


} // namespace gloperate_osg
