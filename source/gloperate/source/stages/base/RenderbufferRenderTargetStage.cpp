
#include <gloperate/stages/base/RenderbufferRenderTargetStage.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Renderbuffer.h>

#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/DepthRenderTarget.h>
#include <gloperate/rendering/StencilRenderTarget.h>
#include <gloperate/rendering/DepthStencilRenderTarget.h>


using namespace gl;
using namespace globjects;


namespace gloperate
{


CPPEXPOSE_COMPONENT(RenderbufferRenderTargetStage, gloperate::Stage)


RenderbufferRenderTargetStage::RenderbufferRenderTargetStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "RenderbufferRenderTargetStage", name)
, internalFormat("internalFormat", this)
, size("size", this)
, renderbuffer("renderbuffer", this)
, colorRenderTarget("colorRenderTarget", this)
, depthRenderTarget("depthRenderTarget", this)
, stencilRenderTarget("stencilRenderTarget", this)
, depthStencilRenderTarget("depthStencilRenderTarget", this)
{
}

RenderbufferRenderTargetStage::~RenderbufferRenderTargetStage()
{
}

void RenderbufferRenderTargetStage::onContextInit(gloperate::AbstractGLContext *)
{
    // Create new texture
    m_renderbuffer = cppassist::make_unique<Renderbuffer>();

    // Create wrapping render target
    m_colorRenderTarget        = cppassist::make_unique<ColorRenderTarget>();
    m_depthRenderTarget        = cppassist::make_unique<DepthRenderTarget>();
    m_stencilRenderTarget      = cppassist::make_unique<StencilRenderTarget>();
    m_depthStencilRenderTarget = cppassist::make_unique<DepthStencilRenderTarget>();
}

void RenderbufferRenderTargetStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_renderbuffer             = nullptr;
    m_colorRenderTarget        = nullptr;
    m_depthRenderTarget        = nullptr;
    m_stencilRenderTarget      = nullptr;
    m_depthStencilRenderTarget = nullptr;
}

void RenderbufferRenderTargetStage::onProcess()
{
    // Check if texture has been created successfully
    if (!m_renderbuffer.get())
    {
        return;
    }

    // Create texture image
    const auto width  = (*size)[2];
    const auto height = (*size)[3];
    m_renderbuffer->storage(*internalFormat, width, height);


    m_colorRenderTarget->releaseTarget();
    m_depthRenderTarget->releaseTarget();
    m_stencilRenderTarget->releaseTarget();
    m_depthStencilRenderTarget->releaseTarget();

    switch(*internalFormat)
    {
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH_COMPONENT24:
    case GL_DEPTH_COMPONENT32F:
        m_depthRenderTarget->setTarget(m_renderbuffer.get());
        break;
    case GL_DEPTH_STENCIL:
    case GL_DEPTH24_STENCIL8:
    case GL_DEPTH32F_STENCIL8:
        m_depthStencilRenderTarget->setTarget(m_renderbuffer.get());
        break;
    case GL_STENCIL:
        m_stencilRenderTarget->setTarget(m_renderbuffer.get());
    default: // Color attachment
        m_colorRenderTarget->setTarget(m_renderbuffer.get());
        break;
    }

    // Update outputs
    renderbuffer.setValue(m_renderbuffer.get());
    colorRenderTarget.setValue(m_colorRenderTarget.get());
    depthRenderTarget.setValue(m_depthRenderTarget.get());
    stencilRenderTarget.setValue(m_stencilRenderTarget.get());
}


} // namespace gloperate
