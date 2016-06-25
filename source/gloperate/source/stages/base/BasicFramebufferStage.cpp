
#include <gloperate/stages/base/BasicFramebufferStage.h>

#include <glbinding/gl/gl.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(BasicFramebufferStage, gloperate::Stage)


BasicFramebufferStage::BasicFramebufferStage(Environment * environment, const std::string & name)
: Stage(environment, name)
, viewport    ("viewport", this)
, fbo         ("fbo", this)
, colorTexture("colorTexture", this)
, depthTexture("depthTexture", this)
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
    // Check if FBO needs to be rebuilt
    if (!fbo.isValid())
    {
        // Rebuild FBO (and textures)
        rebuildFBO();

        // Update outputs
        this->fbo.setValue(m_fbo);
        this->colorTexture.setValue(m_texColor);
        this->depthTexture.setValue(m_texDepth);
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
}


} // namespace gloperate
