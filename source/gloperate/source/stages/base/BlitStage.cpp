
#include <gloperate/stages/base/BlitStage.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/logging.h>

namespace gloperate
{


BlitStage::BlitStage(Environment * environment, const std::string & name)
:Stage(environment, name)
, sourceFBO("sourceFBO", this)
, sourceViewport("sourceViewport", this)
, destinationFBO("destinationFBO", this)
, destinationViewport("destinationViewport", this)
, blittedFBO("blittedFBO", this)
, blitted("blitted", this)
{
}

void BlitStage::onProcess(AbstractGLContext * /*context*/)
{
    globjects::Framebuffer * srcFBO = *sourceFBO;
    if (!srcFBO)
    {
        srcFBO = globjects::Framebuffer::defaultFBO();
    }

    globjects::Framebuffer * destFBO = *destinationFBO;
    if (!destFBO)
    {
        destFBO = globjects::Framebuffer::defaultFBO();
    }

    std::array<gl::GLint, 4> srcRect = {{
        static_cast<gl::GLint>((*sourceViewport).x),
        static_cast<gl::GLint>((*sourceViewport).y),
        static_cast<gl::GLint>((*sourceViewport).z),
        static_cast<gl::GLint>((*sourceViewport).w)
    }};
    std::array<gl::GLint, 4> destRect = {{
        static_cast<gl::GLint>((*destinationViewport).x),
        static_cast<gl::GLint>((*destinationViewport).y),
        static_cast<gl::GLint>((*destinationViewport).z),
        static_cast<gl::GLint>((*destinationViewport).w)
    }};

    srcFBO->blit(gl::GL_COLOR_ATTACHMENT0, srcRect, destFBO, destFBO->id() == 0 ? gl::GL_BACK_LEFT : gl::GL_COLOR_ATTACHMENT0, destRect, gl::GL_COLOR_BUFFER_BIT, gl::GL_LINEAR);

    blittedFBO.setValue(*destinationFBO);
    blitted.setValue(true);
}


} // namespace gloperate
