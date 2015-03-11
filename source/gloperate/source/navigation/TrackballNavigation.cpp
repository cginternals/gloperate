/*
 * TrackballNavigation.cpp
 *
 *  Created on: Mar 11, 2015
 *      Author: Jan Ole Vollmer
 */

#include <gloperate/navigation/TrackballNavigation.h>

#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>

namespace gloperate {

glm::dvec3 operator*(const glm::dmat4& mat, const glm::dvec3& vec) {
    return glm::dvec3(mat * glm::dvec4(vec, 1.0));
}

TrackballNavigation::TrackballNavigation(AbstractCameraCapability * cameraCapability, AbstractViewportCapability * viewportCapability)
: m_cameraCapability(cameraCapability)
, m_viewportCapability(viewportCapability) {
    reset(cameraCapability->eye(), cameraCapability->center(), cameraCapability->up());
}

TrackballNavigation::Mode TrackballNavigation::mode()
{
    return m_mode;
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

    auto rotationMatrix = glm::dmat4(glm::dvec4(s, 0.0), glm::dvec4(u, 0.0), glm::dvec4(-f, 0.0), glm::dvec4(0.0, 0.0, 0.0, 1.0));

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
    pan(glm::dvec2(viewportPosition - m_lastViewportPosition) / glm::dvec2(m_viewportCapability->width(), m_viewportCapability->height()));
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

void TrackballNavigation::updateCamera()
{
    m_cameraCapability->setCenter(glm::vec3(m_center));
    m_cameraCapability->setEye(glm::vec3(m_center + m_trackball.orientationMatrix() * glm::dvec3(0.0, 0.0, m_distance)));
    m_cameraCapability->setUp(glm::vec3(m_trackball.orientationMatrix() * glm::dvec3(0.0, 1.0, 0.0)));
}

} /* namespace gloperate */
