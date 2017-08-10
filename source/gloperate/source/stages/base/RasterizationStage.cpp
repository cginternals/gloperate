
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
    renderInterface.onContextInit();
}

void RasterizationStage::onContextDeinit(AbstractGLContext *)
{
    renderInterface.onContextDeinit();
}

void RasterizationStage::onProcess()
{
    if (!renderInterface.renderTargetsAreCompatible())
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
        auto fbo = renderInterface.obtainFBO();

        // Bind FBO
        fbo->bind(gl::GL_FRAMEBUFFER);

        fbo->printStatus(true);

        // Render the drawable
        (*drawable)->draw();

        // Unbind FBO
        fbo->unbind();
    }

    // Update outputs
    renderInterface.updateRenderTargetOutputs();
}


} // namespace gloperate
