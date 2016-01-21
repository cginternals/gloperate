
#include <gloperate/input/ButtonControl.h>

namespace gloperate
{

void ButtonControl::setPressed(bool state)
{
    if(m_state != state)
    {
        state ? pressed() : released();
        m_state = state;
    }
}

bool ButtonControl::getPressed()
{
    return m_state;
}

}
