
#include <gloperate/stages/base/BlitStage.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/bitfield.h>


namespace gloperate
{


BlitStage::BlitStage(Environment * environment, const std::string & name)
:Stage(environment, name)
, source("source", this)
, sourceViewport("sourceViewport", this)
, target("target", this)
, targetViewport("targetViewport", this)
, minFilter("minFilter", this, gl::GL_LINEAR)
, magFilter("magFilter", this, gl::GL_NEAREST)
, targetOut("targetOut", this)
{
    setAlwaysProcessed(true);
}

void BlitStage::onContextInit(AbstractGLContext * /*context*/)
{
    m_defaultFBO = globjects::Framebuffer::defaultFBO();
    m_sourceFBO  = cppassist::make_unique<globjects::Framebuffer>();
    m_targetFBO  = cppassist::make_unique<globjects::Framebuffer>();
}

void BlitStage::onContextDeinit(AbstractGLContext * /*context*/)
{
    m_defaultFBO = nullptr;
    m_sourceFBO  = nullptr;
    m_targetFBO  = nullptr;
}

void BlitStage::onProcess()
{
    std::array<gl::GLint, 4> sourceRect = {{
        static_cast<gl::GLint>((*sourceViewport).x),
        static_cast<gl::GLint>((*sourceViewport).y),
        static_cast<gl::GLint>((*sourceViewport).z),
        static_cast<gl::GLint>((*sourceViewport).w)
    }};
    std::array<gl::GLint, 4> targetRect = {{
        static_cast<gl::GLint>((*targetViewport).x),
        static_cast<gl::GLint>((*targetViewport).y),
        static_cast<gl::GLint>((*targetViewport).z),
        static_cast<gl::GLint>((*targetViewport).w)
    }};

    globjects::Framebuffer * sourceFBO = source->attachmentRequiresUserDefinedFramebuffer() ? m_sourceFBO.get() : m_defaultFBO.get();
    globjects::Framebuffer * targetFBO = target->attachmentRequiresUserDefinedFramebuffer() ? m_targetFBO.get() : m_defaultFBO.get();
    auto sourceAttachment = source->attachmentRequiresUserDefinedFramebuffer() ? gl::GL_COLOR_ATTACHMENT0 : source->defaultFramebufferAttachment();
    auto targetAttachment = target->attachmentRequiresUserDefinedFramebuffer() ? gl::GL_COLOR_ATTACHMENT0 : target->defaultFramebufferAttachment();

    if (sourceRect[2] <= targetRect[2] && sourceRect[3] <= targetRect[3])
    {
        sourceFBO->blit(sourceAttachment, sourceRect, targetFBO, targetAttachment, targetRect, gl::GL_COLOR_BUFFER_BIT, *magFilter);
    }
    else
    {
        sourceFBO->blit(sourceAttachment, sourceRect, targetFBO, targetAttachment, targetRect, gl::GL_COLOR_BUFFER_BIT, *minFilter);
    }

    targetOut.setValue(*target);
}


} // namespace gloperate
