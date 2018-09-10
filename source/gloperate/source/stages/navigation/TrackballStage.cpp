
#include <gloperate/stages/navigation/TrackballStage.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <cppassist/memory/make_unique.h>

#include <gloperate/base/Environment.h>
#include <gloperate/input/MouseEvent.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(TrackballStage, gloperate::Stage)


TrackballStage::TrackballStage(Environment * environment, const std::string & name)
: Stage(environment, name), AbstractEventConsumer(environment->inputManager())
, viewport("viewport", this)
, defaultPitch("defaultPitch", this, 0.0f)
, defaultYaw("defaultYaw", this, 0.0f)
, defaultZoom("defaultZoom", this, 1.0f)
, camera("camera", this)
, m_camera(cppassist::make_unique<Camera>())
, m_lastMousePosition(0, 0)
, m_dragging(false)
, m_pitch(0.f)
, m_yaw(0.f)
, m_zoom(1.f)
{
}

TrackballStage::~TrackballStage()
{
}

void TrackballStage::onEvent(InputEvent * event)
{
    switch(event->type())
    {
        case InputEvent::Type::MouseButtonPress:
            if (static_cast<MouseEvent*>(event)->button() == MouseButtonLeft)
            {
                // Start dragging
                assert(!m_dragging);
                m_lastMousePosition = static_cast<MouseEvent*>(event)->pos();
                m_dragging = true;
            }
            break;

        case InputEvent::Type::MouseButtonRelease:
            if (static_cast<MouseEvent*>(event)->button() == MouseButtonLeft)
            {
                // Stop dragging
                assert(m_dragging);
                m_dragging = false;
            }
            break;

        case InputEvent::Type::MouseMove:
            // Rotation
            if (m_dragging)
            {
                auto currentMousePosition = static_cast<MouseEvent*>(event)->pos();
                float scaling = 0.15f;

                m_pitch += (m_lastMousePosition.y - currentMousePosition.y) * scaling;
                m_pitch = glm::clamp(m_pitch, -89.f, 89.f);

                m_yaw += (m_lastMousePosition.x - currentMousePosition.x) *scaling;
                m_yaw = m_yaw -180 % 360 + 180;

                m_lastMousePosition = currentMousePosition;
                camera.invalidate();
            }
            break;

        case InputEvent::Type::MouseWheelScroll:
            // Zooming
            if (!m_dragging)
            {
                m_zoom -= static_cast<MouseEvent*>(event)->wheelDelta().y / 3000;
                m_zoom = glm::clamp(m_zoom,0.4f,1.5f);

                camera.invalidate();
            }
            break;

        default:
            break;
    }
}

void TrackballStage::onProcess()
{
    m_camera->setAspectRatio(viewport->z / viewport->w);

    auto mat = glm::yawPitchRoll(glm::radians(*defaultYaw + m_yaw), glm::radians(*defaultPitch + m_pitch), 0.f);
    glm::vec4 rotatedCameraPosition = mat * glm::vec4(0.f, 0.f, 3.f * *defaultZoom * m_zoom, 1.f);

    m_camera->setEye(glm::vec3(rotatedCameraPosition) / rotatedCameraPosition.w);

    camera.setValue(m_camera.get());
}


} // namespace gloperate
