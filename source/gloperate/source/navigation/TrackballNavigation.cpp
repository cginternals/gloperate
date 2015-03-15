#include <gloperate/navigation/TrackballNavigation.h>

#include <iostream>

#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>

namespace gloperate {

TrackballNavigation::TrackballNavigation(AbstractCameraCapability * cameraCapability, AbstractViewportCapability * viewportCapability)
: m_cameraCapability(cameraCapability)
, m_viewportCapability(viewportCapability)
, m_blockCameraSignal(false) {
    m_cameraCapability->changed.connect([this] {
        if (!m_blockCameraSignal) {
            reset();
        }
    });
    reset();
}

TrackballNavigation::Mode TrackballNavigation::mode()
{
    return m_mode;
}

void TrackballNavigation::reset()
{
    reset(m_cameraCapability->eye(), m_cameraCapability->center(), m_cameraCapability->up());
}

void TrackballNavigation::reset(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
    reset(glm::dvec3(eye), glm::dvec3(center), glm::dvec3(up));
}

void TrackballNavigation::reset(const glm::dvec3& eye, const glm::dvec3& center, const glm::dvec3& up)
{
    auto lv = center - eye;
    auto f = glm::normalize(lv);
    auto s = glm::normalize(glm::cross(f, up));
    auto u = glm::normalize(glm::cross(s, f));

    auto rotationMatrix = glm::dmat3(s, u, -f);

    m_center = center;
    m_distance = glm::length(lv);
    m_trackball.reset(glm::inverse(rotationMatrix));

    updateCamera();
}

void TrackballNavigation::panBegin(const glm::ivec2& viewportPosition)
{
    m_mode = Mode::PAN;
    m_lastViewportPosition = viewportPosition;
}

void TrackballNavigation::pan(const glm::ivec2& viewportPosition)
{
    pan(glm::dvec2(viewportPosition - m_lastViewportPosition) / viewport());
    m_lastViewportPosition = viewportPosition;
}

void TrackballNavigation::pan(const glm::dvec2& normalizedDelta)
{
    double scale = -0.3 * m_distance;
    auto scaledDelta = glm::dvec3(normalizedDelta.x * scale, -normalizedDelta.y * scale, 0.0);
    auto rotationMatrix = m_trackball.orientationMatrix();
    m_center += rotationMatrix * scaledDelta;

    updateCamera();
}

void TrackballNavigation::panEnd()
{
    m_mode = Mode::NONE;
}

void TrackballNavigation::rotateBegin(const glm::ivec2& viewportPosition)
{
    m_mode = Mode::ROTATE;
    m_lastViewportPosition = viewportPosition;
}

void TrackballNavigation::rotate(const glm::ivec2& viewportPosition)
{
    rotate(glm::dvec2(m_lastViewportPosition) / viewport() * 2.0 - 1.0, glm::dvec2(viewportPosition) / viewport() * 2.0 - 1.0);
    m_lastViewportPosition = viewportPosition;
}

void TrackballNavigation::rotate(const glm::dvec2& normalizedFrom, const glm::dvec2& normalizedTo)
{
    m_trackball.rotate(normalizedFrom, normalizedTo);

    updateCamera();
}

void TrackballNavigation::rotateEnd()
{
    m_mode = Mode::NONE;
}

void TrackballNavigation::zoom(double delta)
{
    auto scale = 1.0 + delta;
    m_distance *= scale;

    updateCamera();
}

void TrackballNavigation::updateCamera()
{
    m_blockCameraSignal = true;

    m_cameraCapability->setCenter(glm::vec3(m_center));
    m_cameraCapability->setEye(glm::vec3(m_center + m_trackball.orientationMatrix() * glm::dvec3(0.0, 0.0, m_distance)));
    m_cameraCapability->setUp(glm::vec3(m_trackball.orientationMatrix() * glm::dvec3(0.0, 1.0, 0.0)));

    m_blockCameraSignal = false;
}

glm::dvec2 TrackballNavigation::viewport()
{
    return { m_viewportCapability->width(), m_viewportCapability->height() };
}

} /* namespace gloperate */
