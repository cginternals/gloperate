
#include <gloperate/stages/base/ClearStage.h>

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>

#include <gloperate/rendering/AbstractDrawable.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ClearStage, gloperate::Stage)


ClearStage::ClearStage(Environment * environment, const std::string & name)
: Stage(environment, "ClearStage", name)
, renderInterface(this)
, colorTexture   ("colorTexture",    this)
, fboOut         ("fboOut",          this)
, colorTextureOut("colorTextureOut", this)
{
}

ClearStage::~ClearStage()
{
}

void ClearStage::onContextInit(AbstractGLContext *)
{
}

void ClearStage::onProcess()
{
    // Set viewport
    const glm::vec4 & viewport = *renderInterface.deviceViewport;
    gl::glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    // Bind FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    const glm::vec3 & color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Update outputs
    fboOut.setValue(fbo);
    colorTextureOut.setValue(*colorTexture);
}


} // namespace gloperate
