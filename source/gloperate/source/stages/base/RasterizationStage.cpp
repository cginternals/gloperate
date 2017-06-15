
#include <gloperate/stages/base/RasterizationStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>

#include <gloperate/rendering/AbstractDrawable.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(RasterizationStage, gloperate::Stage)


RasterizationStage::RasterizationStage(Environment * environment, const std::string & name)
: Stage(environment, "RasterizationStage", name)
, renderInterface(             this)
, rasterize      ("rasterize", this, true)
, drawable       ("drawable",  this)
{
}

RasterizationStage::~RasterizationStage()
{
}

void RasterizationStage::onContextInit(AbstractGLContext *)
{
    m_defaultFBO = globjects::Framebuffer::defaultFBO();
    m_fbo = cppassist::make_unique<globjects::Framebuffer>();
}

void RasterizationStage::onContextDeinit(AbstractGLContext *)
{
    m_defaultFBO = nullptr;
    m_fbo = nullptr;
}

void RasterizationStage::onProcess()
{
    if (!renderInterface.allRenderTargetsCompatible())
    {
        cppassist::warning("gloperate") << "Framebuffer attachments not compatible";

        return;
    }

    // Check if rasterization is enabled
    if (*rasterize)
    {
        // Set viewport
        const glm::vec4 & viewport = *renderInterface.viewport;
        gl::glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        // Configure FBO
        auto fbo = renderInterface.configureFBO(m_fbo.get(), m_defaultFBO.get());

        // Bind FBO
        fbo->bind(gl::GL_FRAMEBUFFER);

        fbo->printStatus(true);

        // Render the drawable
        (*drawable)->draw();

        // Unbind FBO
        fbo->unbind();
    }

    // Update outputs
    renderInterface.pairwiseRenderTargetsDo([](Input <RenderTarget *> * input, Output <RenderTarget *> * output) {
        output->setValue(**input);
    });
}


} // namespace gloperate
