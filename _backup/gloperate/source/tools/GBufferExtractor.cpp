
#include <gloperate/tools/GBufferExtractor.h>

#include <gloperate/ext-includes-begin.h>
#include <glm/gtc/type_ptr.hpp>
#include <gloperate/ext-includes-end.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>
#include <gloperate/base/RenderTarget.h>


namespace gloperate
{


GBufferExtractor::GBufferExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability,
    RenderTargetType type,
    const glm::vec4 & defaultValue)
:   m_viewportCapability(viewportCapability)
,   m_typedRenderTargetCapability(typedRenderTargetCapability)
,   m_type(type)
,   m_defaultValue(defaultValue)
{
}

GBufferExtractor::~GBufferExtractor()
{
}

glm::vec4 GBufferExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    if (!m_typedRenderTargetCapability || !m_viewportCapability)
    {
        return m_defaultValue;
    }

    if (!m_typedRenderTargetCapability->hasRenderTarget(m_type))
        return m_defaultValue;

    const gloperate::RenderTarget & target = m_typedRenderTargetCapability->renderTarget(m_type);

    const gl::GLint x = static_cast<gl::GLint>(m_viewportCapability->x());
    const gl::GLint y = static_cast<gl::GLint>(m_viewportCapability->y());
    const gl::GLint w = static_cast<gl::GLint>(m_viewportCapability->width());
    const gl::GLint h = static_cast<gl::GLint>(m_viewportCapability->height());

    if (x > windowCoordinates.x || windowCoordinates.x >= w || y > windowCoordinates.y || windowCoordinates.y >= h)
        return m_defaultValue;

    target.framebuffer()->bind(gl::GL_READ_FRAMEBUFFER);
    gl::glBindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

    if (target.attachment() != gl::GL_DEPTH_ATTACHMENT)
        gl::glReadBuffer(target.attachment()); // glReadBuffer does not accept GL_DEPTH_ATTACHMENT and causes an error

    glm::vec4 value;

    gl::glReadPixels(windowCoordinates.x, h - windowCoordinates.y, 1, 1, target.format(), gl::GL_FLOAT, reinterpret_cast<void*>(glm::value_ptr(value)));

    gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);

    return value;
}


} // namespace gloperate
