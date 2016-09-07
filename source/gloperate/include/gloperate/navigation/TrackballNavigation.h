#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/navigation/Trackball.h>

namespace gloperate {

class AbstractCameraCapability;
class AbstractViewportCapability;

class GLOPERATE_API TrackballNavigation {
public:
    enum class Mode {
        NONE,
        PAN,
        ROTATE
    };

public:
    TrackballNavigation(AbstractCameraCapability * cameraCapability, AbstractViewportCapability * viewportCapability);

    Mode mode();

    void reset();
    void reset(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    void reset(const glm::dvec3& eye, const glm::dvec3& center, const glm::dvec3& up);

    void panBegin(const glm::ivec2& viewportPosition);
    void pan(const glm::ivec2& viewportPosition);
    void pan(const glm::dvec2& normalizedDelta);
    void panEnd();

    void rotateBegin(const glm::ivec2& viewportPosition);
    void rotate(const glm::ivec2& viewportPosition);
    void rotate(const glm::dvec2& normalizedFrom, const glm::dvec2& normalizedTo);
    void rotateEnd();

    void zoom(double delta);

protected:
    void updateCamera();
    glm::dvec2 viewport();

protected:
    Trackball m_trackball;
    AbstractCameraCapability * m_cameraCapability;
    AbstractViewportCapability * m_viewportCapability;
    bool m_blockCameraSignal;
    Mode m_mode;
    double m_distance;
    glm::dvec3 m_center;
    glm::ivec2 m_lastViewportPosition;
};

} /* namespace gloperate */
