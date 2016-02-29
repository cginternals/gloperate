
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
      , MouseButtonPress
      , MouseButtonRelease
      , MouseWheelScroll
    };

public:
    InputEvent(Type type, AbstractDevice * dispatchingDevice);
    virtual ~InputEvent();

    Type type() const;

protected:
    Type m_type;
    AbstractDevice * m_dispatchingDevice;
};

class GLOPERATE_API MouseEvent : public InputEvent
{
public:
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, int button = 0);
    MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, glm::vec2 wheelDelta);
    int x() const;
    int y() const;
    int button() const;
    glm::vec2 wheelDelta() const;

protected:
    glm::ivec2 m_pos;
    int m_button;
    glm::vec2 m_wheelDelta;
};

class GLOPERATE_API ButtonEvent : public InputEvent
{
public:
    ButtonEvent(Type type, AbstractDevice * dispatchingDevice, const std::string & description);
    std::string description() const;

protected:
    std::string m_description;
};

} //namespace gloperate
