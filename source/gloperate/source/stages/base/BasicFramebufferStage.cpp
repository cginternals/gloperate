
#include <gloperate/stages/base/BasicFramebufferStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/RenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(BasicFramebufferStage, gloperate::Stage)


BasicFramebufferStage::BasicFramebufferStage(Environment * environment, const std::string & name)
: Stage(environment, "BasicFramebufferStage", name)
, viewport    ("viewport", this)
, colorTexture("colorTexture", this)
, depthTexture("depthTexture", this)
, colorBuffer ("colorBuffer", this)
, depthBuffer ("depthBuffer", this)
{
}

BasicFramebufferStage::~BasicFramebufferStage()
{
}

void BasicFramebufferStage::onContextInit(AbstractGLContext *)
{
    // Create color texture
    m_colorTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    m_colorBuffer = cppassist::make_unique<RenderTarget>();
    m_colorBuffer->setTarget(m_colorTexture.get());
    m_colorBuffer->setAttachmentType(AttachmentType::Color);

    // Create depth texture
    m_depthTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    m_depthBuffer = cppassist::make_unique<RenderTarget>();
    m_depthBuffer->setTarget(m_depthTexture.get());
    m_depthBuffer->setAttachmentType(AttachmentType::Depth);
}

void BasicFramebufferStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_colorBuffer  = nullptr;
    m_depthBuffer  = nullptr;
    m_colorTexture = nullptr;
    m_depthTexture = nullptr;
}

void BasicFramebufferStage::onProcess()
{
    // Get texture size
    glm::ivec2 size = glm::ivec2((*this->viewport).z, (*this->viewport).w);

    cppassist::debug() << "resize " << m_colorTexture->id();

    m_colorTexture->image2D(0, gl::GL_RGBA, size.x, size.y, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    cppassist::debug() << "resize " << m_depthTexture->id();

    m_depthTexture->image2D(0, gl::GL_DEPTH_COMPONENT, size.x, size.y, 0, gl::GL_DEPTH_COMPONENT, gl::GL_UNSIGNED_BYTE, nullptr);

    // Update outputs
    this->colorTexture.setValue(m_colorTexture.get());
    this->depthTexture.setValue(m_depthTexture.get());
    this->colorBuffer.setValue(m_colorBuffer.get());
    this->depthBuffer.setValue(m_depthBuffer.get());
}


} // namespace gloperate
