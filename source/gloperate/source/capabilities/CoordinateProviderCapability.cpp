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
    ,   m_viewportCapability(viewportCapability)
    ,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
    m_typedRenderTargetCapability->changed.connect([this](){this->onRenderTargetsChanged();});
    onRenderTargetsChanged();
}

CoordinateProviderCapability::~CoordinateProviderCapability()
{
}

float CoordinateProviderCapability::depthAt(const glm::ivec2 & windowCoordinates) const
{
    if (!m_depthBuffer.isValid()) 
        return 1.f;

    const gl::GLint x = windowCoordinates.x;
    const gl::GLint y = windowCoordinates.y;
    
    const gl::GLint w = static_cast<gl::GLint>(m_viewportCapability->width()); 
    const gl::GLint h = static_cast<gl::GLint>(m_viewportCapability->height());

    if (x >= w || y >= h)
        return 1.f;
    
    gl::glBindFramebuffer(gl::GLenum::GL_READ_FRAMEBUFFER, m_depthBuffer.framebuffer().get()->id());
    gl::glReadBuffer(m_depthBuffer.attachment());
   
    gl::GLfloat z;
    gl::GLenum format = m_depthBuffer.format();
    gl::glReadPixels(x, h - y - 1, 1, 1, format, gl::GLenum::GL_FLOAT, reinterpret_cast<void*>(&z));
    
    gl::glBindFramebuffer(gl::GLenum::GL_READ_FRAMEBUFFER, 0);

    return z;
}

//virtual glm::vec3 worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const override;

glm::vec3 CoordinateProviderCapability::worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const
{
    const float depth = depthAt(windowCoordinates);

    return unproject(windowCoordinates, depth);
}

glm::vec3 CoordinateProviderCapability::unproject(const glm::ivec2 & windowCoordinates, float depth) const
{
    const glm::mat4 viewProjectionInverted = m_cameraCapability->viewInverted() * m_projectionCapability->projectionInverted();

    const float x = static_cast<float>(windowCoordinates.x);
    const float y = static_cast<float>(windowCoordinates.y);

    // transform viewport to [-1;+1] (including z!)

    const float width = 2.0f / static_cast<float>(m_viewportCapability->width());
    const float height = 2.0f / static_cast<float>(m_viewportCapability->height());

    const glm::vec4 p = glm::vec4(x * width - 1.f, 1.f - y * height, depth * 2.f - 1.f, 1.f);

    // unproject this point back to object space
    const glm::vec4 u = viewProjectionInverted * p;
    return glm::vec3(u) / u.w;
}

void CoordinateProviderCapability::onRenderTargetsChanged()
{
    m_depthBuffer = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Depth);
    m_geometryBuffer = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Geometry);
}

} // namespace gloperate
