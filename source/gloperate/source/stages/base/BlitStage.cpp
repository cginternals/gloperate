
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
, targetFBO("targetFBO", this)
, targetViewport("targetViewport", this)
, fboOut("fboOut", this)
, rendered("rendered", this)
{
}

void BlitStage::onProcess()
{
    globjects::Framebuffer * srcFBO = *sourceFBO;

    globjects::Framebuffer * destFBO = *targetFBO;

    std::array<gl::GLint, 4> srcRect = {{
        static_cast<gl::GLint>((*sourceViewport).x),
        static_cast<gl::GLint>((*sourceViewport).y),
        static_cast<gl::GLint>((*sourceViewport).z),
        static_cast<gl::GLint>((*sourceViewport).w)
    }};
    std::array<gl::GLint, 4> destRect = {{
        static_cast<gl::GLint>((*targetViewport).x),
        static_cast<gl::GLint>((*targetViewport).y),
        static_cast<gl::GLint>((*targetViewport).z),
        static_cast<gl::GLint>((*targetViewport).w)
    }};

    srcFBO->blit(gl::GL_COLOR_ATTACHMENT0, srcRect, destFBO, destFBO->id() == 0 ? gl::GL_BACK_LEFT : gl::GL_COLOR_ATTACHMENT0, destRect, gl::GL_COLOR_BUFFER_BIT, gl::GL_LINEAR);

    fboOut.setValue(*targetFBO);
    rendered.setValue(true);
}


} // namespace gloperate
