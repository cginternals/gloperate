
#include <gloperate/stages/base/BasicFramebufferStage.h>

#include <glbinding/gl/gl.h>

#include <glm/vec2.hpp>

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
, viewport    ("viewport",     this)
, colorTexture("colorTexture", this)
, depthTexture("depthTexture", this)
, colorBuffer ("colorBuffer",  this)
, depthBuffer ("depthBuffer",  this)
{
}

BasicFramebufferStage::~BasicFramebufferStage()
{
}

void BasicFramebufferStage::onContextInit(AbstractGLContext *)
{
    m_colorBuffer = cppassist::make_unique<ColorRenderTarget>();

    m_depthBuffer = cppassist::make_unique<DepthRenderTarget>();

    // Create color texture
    m_colorTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    // Create depth texture
    m_depthTexture = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);

    m_colorBuffer->setTarget(m_colorTexture.get());
    m_depthBuffer->setTarget(m_depthTexture.get());
}

void BasicFramebufferStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_colorBuffer         = nullptr;
    m_depthBuffer         = nullptr;
    m_colorTexture        = nullptr;
    m_depthTexture = nullptr;
}

void BasicFramebufferStage::onProcess()
{
    // Get texture size
    glm::ivec2 size = glm::ivec2((*this->viewport).z, (*this->viewport).w);

    m_colorTexture->image2D(0, gl::GL_RGBA, size.x, size.y, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, nullptr);

    m_depthTexture->image2D(0, gl::GL_DEPTH_COMPONENT, size.x, size.y, 0, gl::GL_DEPTH_COMPONENT, gl::GL_FLOAT, nullptr);

    // Update outputs
    this->colorTexture.setValue(m_colorTexture.get());
    this->depthTexture.setValue(m_depthTexture.get());
    this->colorBuffer.setValue(m_colorBuffer.get());
    this->depthBuffer.setValue(m_depthBuffer.get());
}


} // namespace gloperate
