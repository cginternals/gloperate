
#include <gloperate/stages/base/RenderbufferRenderTargetStage.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <glbinding/gl/enum.h>

#include <globjects/Renderbuffer.h>

#include <gloperate/rendering/RenderTarget.h>


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
, renderTarget("renderTarget", this)
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
    m_renderTarget = cppassist::make_unique<RenderTarget>();
    m_renderTarget->setTarget(m_renderbuffer.get());
}

void RenderbufferRenderTargetStage::onContextDeinit(AbstractGLContext *)
{
    // Clean up OpenGL objects
    m_renderbuffer = nullptr;
    m_renderTarget = nullptr;
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

    // Update outputs
    renderbuffer.setValue(m_renderbuffer.get());
    renderTarget.setValue(m_renderTarget.get());
}


} // namespace gloperate
