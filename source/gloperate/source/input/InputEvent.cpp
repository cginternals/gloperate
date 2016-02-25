
#include <cassert>

#include <gloperate/input/InputEvent.h>

namespace gloperate{

InputEvent::InputEvent(Type type)
:  m_type(type)
{

}

InputEvent::~InputEvent()
{

}

InputEvent::Type InputEvent::type() const
{
    return m_type;
}


MouseMoveEvent::MouseMoveEvent(Type type, glm::ivec2 pos)
: InputEvent(type)
, m_pos(pos)
{

}

MouseMoveEvent::MouseMoveEvent(Type type, int x, int y)
: MouseMoveEvent(type, glm::ivec2(x,y))
{

}

int MouseMoveEvent::x() const
{
    return m_pos.x;
}

int MouseMoveEvent::y() const
{
    return m_pos.y;
}


ButtonEvent::ButtonEvent(Type type,const std::string & description)
: InputEvent(type)
, m_description(description)
{

}

} //namespace gloperate
