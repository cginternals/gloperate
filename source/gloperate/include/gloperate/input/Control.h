
#pragma once

namespace gloperate{

class InputEvent;
class InputManager;

class Control{

public:

    Control(unsigned int deviceId, unsigned int id, InputManager * inputManager);

    void dispatch(InputEvent * event);

    unsigned int deviceId();
    unsigned int id();

private:
    unsigned int m_deviceId;
    unsigned int m_id;
    InputManager * m_inputManager;
};

} // namespace gloperate
