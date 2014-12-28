#include <gloperate/tools/NormalExtractor.h>

#include <glm/gtc/type_ptr.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>

#include <gloperate/base/RenderTarget.h>

namespace gloperate
{

NormalExtractor::NormalExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   m_viewportCapability(viewportCapability)
,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
}

NormalExtractor::~NormalExtractor()
{
}

glm::vec3 NormalExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    if (!m_typedRenderTargetCapability || !m_viewportCapability)
    {
        return glm::vec3(0.0);
    }

    if (!m_typedRenderTargetCapability->hasRenderTarget(RenderTargetType::Normal))
        return glm::vec3(0.0);

    const gloperate::RenderTarget & normalTarget = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Normal);

    const gl::GLint x = static_cast<gl::GLint>(m_viewportCapability->x());
    const gl::GLint y = static_cast<gl::GLint>(m_viewportCapability->y());
    const gl::GLint w = static_cast<gl::GLint>(m_viewportCapability->width());
    const gl::GLint h = static_cast<gl::GLint>(m_viewportCapability->height());

    if (x > windowCoordinates.x || windowCoordinates.x >= w || y > windowCoordinates.y || windowCoordinates.y >= h)
        return glm::vec3(0.0);

    normalTarget.framebuffer()->bind(gl::GL_READ_FRAMEBUFFER);
    gl::glBindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

    if (normalTarget.attachment() != gl::GL_DEPTH_ATTACHMENT)
        gl::glReadBuffer(normalTarget.attachment()); // glReadBuffer does not accept GL_DEPTH_ATTACHMENT and causes an error

    glm::vec3 normal;

    gl::glReadPixels(windowCoordinates.x, h - windowCoordinates.y, 1, 1, normalTarget.format(), gl::GL_FLOAT, reinterpret_cast<void*>(glm::value_ptr(normal)));

    gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);

    return normal * glm::vec3(2.0) - glm::vec3(1.0);
}

} // namespace gloperate
