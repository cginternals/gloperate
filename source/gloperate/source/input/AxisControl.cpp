
#include <gloperate/input/AxisControl.h>

namespace gloperate
{

void AxisControl::setPosition(float position)
{
    m_position = position;
    changed(position);
}

}
