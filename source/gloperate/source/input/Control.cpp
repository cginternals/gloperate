
#include <gloperate/input/Control.h>

#include <gloperate/input/InputManager.h>

namespace gloperate{

Control::Control(unsigned int deviceId, unsigned int id, InputManager * inputManager)
: m_deviceId(deviceId)
, m_id(id)
, m_inputManager(inputManager)
{

}

void Control::dispatch(InputEvent * event)
{
    m_inputManager->onControlEvent(this, event);
}

unsigned int Control::deviceId()
{
    return m_deviceId;
}

unsigned int Control::id()
{
    return m_id;
}

} // namespace gloperate
