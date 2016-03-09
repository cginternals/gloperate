
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
      , SpatialAxis
    };

public:
    InputEvent(Type type, AbstractDevice * dispatchingDevice);
    virtual ~InputEvent();

    Type type() const;
    virtual std::string asString() const;

protected:
    Type m_type;
    AbstractDevice * m_dispatchingDevice;
};


} //namespace gloperate
