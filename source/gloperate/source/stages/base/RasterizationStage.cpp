
#include <gloperate/stages/base/RasterizationStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>

#include <gloperate/rendering/AbstractDrawable.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(RasterizationStage, gloperate::Stage)


RasterizationStage::RasterizationStage(Environment * environment, const std::string & name)
: Stage(environment, "RasterizationStage", name)
, renderInterface(this)
, rasterize      ("rasterize",       this, true)
, drawable       ("drawable",        this)
, colorTexture   ("colorTexture",    this)
, fboOut         ("fboOut",          this)
, colorTextureOut("colorTextureOut", this)
{
}

RasterizationStage::~RasterizationStage()
{
}

void RasterizationStage::onProcess()
{
    if (!*rasterize)
    {
        return;
    }

    // Set viewport
    const glm::vec4 & viewport = *renderInterface.deviceViewport;
    gl::glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    // Bind FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Render the drawable
    (*drawable)->draw();

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Update outputs
    fboOut.setValue(fbo);
    colorTextureOut.setValue(*colorTexture);
}


} // namespace gloperate
