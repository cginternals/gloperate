#include <gloperate/tools/ObjectIdExtractor.h>

#include <glm/common.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractTypedRenderTargetCapability.h>

#include <gloperate/base/RenderTarget.h>

namespace gloperate
{

ObjectIdExtractor::ObjectIdExtractor(
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
:   m_viewportCapability(viewportCapability)
,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
}

ObjectIdExtractor::~ObjectIdExtractor()
{
}

int ObjectIdExtractor::get(const glm::ivec2 & windowCoordinates) const
{
    if (!m_typedRenderTargetCapability || !m_viewportCapability)
    {
        return -1;
    }

    if (!m_typedRenderTargetCapability->hasRenderTarget(RenderTargetType::ObjectID))
        return -1;

    const gloperate::RenderTarget & objectIdTarget = m_typedRenderTargetCapability->renderTarget(RenderTargetType::ObjectID);

    const gl::GLint x = static_cast<gl::GLint>(m_viewportCapability->x());
    const gl::GLint y = static_cast<gl::GLint>(m_viewportCapability->y());
    const gl::GLint w = static_cast<gl::GLint>(m_viewportCapability->width());
    const gl::GLint h = static_cast<gl::GLint>(m_viewportCapability->height());

    if (x > windowCoordinates.x || windowCoordinates.x >= w || y > windowCoordinates.y || windowCoordinates.y >= h)
        return -1;

    objectIdTarget.framebuffer()->bind(gl::GL_READ_FRAMEBUFFER);
    gl::glBindBuffer(gl::GL_PIXEL_PACK_BUFFER, 0);

    if (objectIdTarget.attachment() != gl::GL_DEPTH_ATTACHMENT)
        gl::glReadBuffer(objectIdTarget.attachment()); // glReadBuffer does not accept GL_DEPTH_ATTACHMENT and causes an error

    gl::GLfloat id = -1.0;

    gl::glReadPixels(windowCoordinates.x, h - windowCoordinates.y, 1, 1, objectIdTarget.format(), gl::GL_FLOAT, reinterpret_cast<void*>(&id));

    gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);

    return static_cast<int>(glm::floor(id + 0.5));
}

} // namespace gloperate
