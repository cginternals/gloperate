#include <gloperate/capabilities/CoordinateProviderCapability.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <glm/glm.hpp>

#include <gloperate/Camera.h>

namespace gloperate
{
CoordinateProviderCapability::CoordinateProviderCapability(
    AbstractCameraCapability * cameraCapability,
    AbstractProjectionCapability * projectionCapability,
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
    :   AbstractCoordinateProviderCapability()
    ,   m_cameraCapability(cameraCapability)
    ,   m_projectionCapability(projectionCapability)
    , m_viewportCapability(viewportCapability)
    ,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
    m_depthBuffer = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Depth);
    m_geometryBuffer = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Geometry);
}

float CoordinateProviderCapability::depthAt(const glm::ivec2 & windowCoordinates) const
{
    if (!m_depthBuffer.isValid()) 
        return 1.f;

    const gl::GLint x = windowCoordinates.x;
    const gl::GLint y = windowCoordinates.y;
    
    const gl::GLint w = static_cast<gl::GLint>(m_viewportCapability->x());
    const gl::GLint h = static_cast<gl::GLint>(m_viewportCapability->y());

    if (x >= w || y >= h)
        return 1.f;

    gl::glBindFramebuffer(gl::GLenum::GL_READ_FRAMEBUFFER, m_depthBuffer.framebuffer().get()->id());
    gl::glReadBuffer(m_depthBuffer.attachment());

    gl::GLfloat z;
    gl::glReadPixels(x, h - y - 1, 1, 1, m_depthBuffer.format(), gl::GL_FLOAT, reinterpret_cast<void*>(&z));

    gl::glBindFramebuffer(gl::GLenum::GL_READ_FRAMEBUFFER, 0);

    return z;
}

//virtual glm::vec3 worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const override;

glm::vec3 CoordinateProviderCapability::worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const
{
    const glm::mat4 viewProjectionInverted = m_cameraCapability->viewInverted() * m_projectionCapability->projectionInverted();
    const float depth = depthAt(windowCoordinates);

    const float x = static_cast<float>(windowCoordinates.x);
    const float y = static_cast<float>(windowCoordinates.y);

    // transform viewport to [-1;+1] (including z!)
    
    const float w = 2.0f / static_cast<float>(m_viewportCapability->x());
    const float h = 2.0f / static_cast<float>(m_viewportCapability->y());

    const glm::vec4 p = glm::vec4(x * w - 1.f, 1.f - y * h, depth * 2.f - 1.f, 1.f);

    // unproject this point back to object space
    const glm::vec4 u = viewProjectionInverted * p;
    return glm::vec3(u) / u.w;
}

} // namespace globjectsutils
