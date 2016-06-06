
#pragma once

namespace gloperate{

class InputEvent;

class Control{

    Control(unsigned int id);

    int id();

private:
    unsigned int m_id;
};

} // namespace gloperate
