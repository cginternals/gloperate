
#include <gloperate/stages/base/BasicFramebufferStage.h>

#include <glbinding/gl/gl.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


BasicFramebufferStage::BasicFramebufferStage(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: Stage(viewerContext, name, parent)
, viewport    (this, "viewport")
, fbo         (this, "fbo")
, colorTexture(this, "colorTexture")
, depthTexture(this, "depthTexture")
{
}

BasicFramebufferStage::~BasicFramebufferStage()
{
}

void BasicFramebufferStage::onContextInit(AbstractGLContext *)
{
}

void BasicFramebufferStage::onContextDeinit(AbstractGLContext *)
{
}

void BasicFramebufferStage::onProcess(AbstractGLContext *)
{
    // Check if FBO has to be rebuilt
    if (!m_fbo.get()) {
        rebuildFBO();
    }
}

void BasicFramebufferStage::rebuildFBO()
{
    // Get texture size
    glm::ivec2 size = glm::ivec2((*this->viewport).z, (*this->viewport).w);

    // Create color texture
    m_texColor = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_texColor->image2D(0, gl::GL_RGBA, size.x, size.y, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    // Create depth texture
    m_texDepth = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_texDepth->image2D(0, gl::GL_DEPTH_COMPONENT, size.x, size.y, 0, gl::GL_DEPTH_COMPONENT, gl::GL_UNSIGNED_BYTE, nullptr);

    // Create FBO
    m_fbo = new globjects::Framebuffer;
    m_fbo->setDrawBuffers({ gl::GL_COLOR_ATTACHMENT0 });
    m_fbo->attachTexture(gl::GL_COLOR_ATTACHMENT0, m_texColor.get());
    m_fbo->attachTexture(gl::GL_DEPTH_ATTACHMENT,  m_texDepth.get());

    // Update outputs
    this->fbo.setValue(m_fbo);
    this->colorTexture.setValue(m_texColor);
    this->depthTexture.setValue(m_texDepth);
}


CPPEXPOSE_COMPONENT(
    BasicFramebufferStage, gloperate::Stage
  , ""   // Tags
  , ""   // Icon
  , ""   // Annotations
  , "Stage that maintains a basic framebuffer with a color and depth attachment"
  , GLOPERATE_AUTHOR_ORGANIZATION
  , "v1.0.0"
)


} // namespace gloperate
