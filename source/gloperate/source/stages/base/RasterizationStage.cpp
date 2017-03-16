
#include <gloperate/stages/base/RasterizationStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Program.h>
#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>

#include <gloperate/rendering/RenderPass.h>
#include <gloperate/rendering/Drawable.h>

#include <gloperate/gloperate.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(RasterizationStage, gloperate::Stage)


RasterizationStage::RasterizationStage(Environment * environment, const std::string & name)
: Stage(environment, "RasterizationStage", name)
, renderInterface (this)
, rasterize       ("rasterize",       this, true)
//, drawable        ("drawable",        this)
//, program         ("program",         this)
, renderPass      ("renderPass",      this)
, colorTexture    ("colorTexture",    this)
, fboOut          ("fboOut",          this)
, colorTextureOut ("colorTextureOut", this)
{
}

RasterizationStage::~RasterizationStage()
{
}

void RasterizationStage::onContextInit(AbstractGLContext *)
{
}

void RasterizationStage::onProcess(AbstractGLContext *)
{
    if (!*rasterize)
    {
        return;
    }

    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    const glm::vec4 & viewport = *renderInterface.deviceViewport;

    gl::glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    // Clear background
    const glm::vec3 & color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // bind FBO
    fbo->bind(gl::GL_FRAMEBUFFER);

    // draw with the RenderPass
    (*renderPass)->draw();

    // unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    fboOut.setValue(fbo);
    colorTextureOut.setValue(*colorTexture);
}


} // namespace gloperate
