#include <gloperate/tools/CoordinateProvider.h>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <glm/glm.hpp>

#include <gloperate/painter/Camera.h>

namespace gloperate
{
CoordinateProvider::CoordinateProvider(
    AbstractCameraCapability * cameraCapability,
    AbstractProjectionCapability * projectionCapability,
    AbstractViewportCapability * viewportCapability,
    AbstractTypedRenderTargetCapability * typedRenderTargetCapability)
    :   m_cameraCapability(cameraCapability)
    ,   m_projectionCapability(projectionCapability)
    ,   m_viewportCapability(viewportCapability)
    ,   m_typedRenderTargetCapability(typedRenderTargetCapability)
{
    m_typedRenderTargetCapability->changed.connect([this](){this->onRenderTargetsChanged();});
    onRenderTargetsChanged();
}

CoordinateProvider::~CoordinateProvider()
{
}

float CoordinateProvider::depthAt(const glm::ivec2 & windowCoordinates) const
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

    if (m_depthBuffer.attachment() != gl::GLenum::GL_DEPTH_ATTACHMENT)
        gl::glReadBuffer(m_depthBuffer.attachment()); // glReadBuffer does not accept GL_DEPTH_ATTACHMENT and causes an error

    gl::GLfloat z;

    gl::GLenum format = m_depthBuffer.format();
    gl::glReadPixels(x, h - y - 1, 1, 1, format, gl::GLenum::GL_FLOAT, reinterpret_cast<void*>(&z));

    gl::glBindFramebuffer(gl::GLenum::GL_READ_FRAMEBUFFER, 0);

    return z;
}

bool CoordinateProvider::validDepth(const float depth)
{
    return depth < (1.f - std::numeric_limits<float>::epsilon());
}

glm::vec3 CoordinateProvider::worldCoordinatesAt(const glm::ivec2 & windowCoordinates) const
{
    const float depth = depthAt(windowCoordinates);

    return unproject(windowCoordinates, depth);
}

glm::vec3 CoordinateProvider::unproject(const glm::ivec2 & windowCoordinates, float depth) const
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

void CoordinateProvider::onRenderTargetsChanged()
{
    m_depthBuffer = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Depth);
    m_geometryBuffer = m_typedRenderTargetCapability->renderTarget(RenderTargetType::Geometry);
}

} // namespace gloperate
