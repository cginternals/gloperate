
#pragma once

#include <string>

#include <gloperate/gloperate_api.h>
#include <gloperate/viewer/input.h>

#include <glm/glm.hpp>

namespace gloperate{


    class AbstractDevice;

class GLOPERATE_API InputEvent
{
public:
    enum class Type
    {
        ButtonPress
      , ButtonRelease
      , MouseMove
    };

public:
    InputEvent(Type type);
    InputEvent(Type type, AbstractDevice * dispatchingDevice);
    virtual ~InputEvent();

    Type type() const;

protected:
    Type m_type;
    AbstractDevice * m_dispatchingDevice;
};

class GLOPERATE_API MouseMoveEvent : public InputEvent
class GLOPERATE_API MouseEvent : public InputEvent
{
public:
    MouseMoveEvent(Type type, int x, int y);
    MouseMoveEvent(Type type, glm::ivec2 pos);
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, int button = 0);
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, glm::vec2 wheelDelta);
    int x() const;
    int y() const;

protected:
    glm::ivec2 m_pos;
};

class GLOPERATE_API ButtonEvent : public InputEvent
{
public:
    ButtonEvent(Type type, const std::string & description);
    ButtonEvent(Type type, AbstractDevice * dispatchingDevice, const std::string & description);
    std::string description() const;

protected:
    std::string m_description;

};

} //namespace gloperate
