
#include <gloperate/stages/base/BasicFramebufferStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>

#include <gloperate/rendering/AbstractRenderTarget.h>
#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/DepthRenderTarget.h>
#include <gloperate/rendering/StencilRenderTarget.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(BasicFramebufferStage, gloperate::Stage)


BasicFramebufferStage::BasicFramebufferStage(Environment * environment, const std::string & name)
: Stage(environment, "BasicFramebufferStage", name)
, viewport           ("viewport",            this)
, colorTexture       ("colorTexture",        this)
, depthStencilTexture("depthStencilTexture", this)
, colorBuffer        ("colorBuffer",         this)
, depthBuffer        ("depthBuffer",         this)
, stencilBuffer      ("stencilBuffer",       this)
{
}

BasicFramebufferStage::~BasicFramebufferStage()
{
}

void BasicFramebufferStage::onContextInit(AbstractGLContext *)
{
    m_colorBuffer = cppassist::make_unique<ColorRenderTarget>();
    m_colorBuffer->setAttachmentType(AttachmentType::Color);

    m_depthBuffer = cppassist::make_unique<DepthRenderTarget>();
    m_depthBuffer->setAttachmentType(AttachmentType::DepthStencil);

    m_stencilBuffer = cppassist::make_unique<StencilRenderTarget>();
    m_stencilBuffer->setAttachmentType(AttachmentType::DepthStencil);

    // Create color texture
    m_colorTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    // Create depth texture
    m_depthStencilTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    m_colorBuffer->setTarget(m_colorTexture.get());
    m_depthBuffer->setTarget(m_depthStencilTexture.get());
    m_stencilBuffer->setTarget(m_depthStencilTexture.get());
}

void BasicFramebufferStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_colorBuffer         = nullptr;
    m_depthBuffer         = nullptr;
    m_stencilBuffer       = nullptr;
    m_colorTexture        = nullptr;
    m_depthStencilTexture = nullptr;
}

void BasicFramebufferStage::onProcess()
{
    // Get texture size
    glm::ivec2 size = glm::ivec2((*this->viewport).z, (*this->viewport).w);

    cppassist::debug() << "resize " << m_colorTexture->id();

    m_colorTexture->image2D(0, gl::GL_RGBA, size.x, size.y, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    cppassist::debug() << "resize " << m_depthStencilTexture->id();

    m_depthStencilTexture->image2D(0, gl::GL_DEPTH24_STENCIL8, size.x, size.y, 0, gl::GL_DEPTH_STENCIL, gl::GL_UNSIGNED_BYTE, nullptr);

    // Update outputs
    this->colorTexture.setValue(m_colorTexture.get());
    this->depthStencilTexture.setValue(m_depthStencilTexture.get());
    this->colorBuffer.setValue(m_colorBuffer.get());
    this->depthBuffer.setValue(m_depthBuffer.get());
    this->stencilBuffer.setValue(m_stencilBuffer.get());
}


} // namespace gloperate
