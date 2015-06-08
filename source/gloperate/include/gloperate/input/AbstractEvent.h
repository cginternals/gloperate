#pragma once

#include <gloperate/gloperate_api.h>


namespace gloperate
{

enum class SourceType : char
{
    Unknown
,   Keyboard
,   Mouse
,   Timer
,   Wheel
,   Frame
};

class GLOPERATE_API AbstractEvent
{
public:
    virtual ~AbstractEvent();

    SourceType sourceType() const;

protected:
    AbstractEvent();

protected:
    SourceType m_sourceType;

};

} // namespace gloperate
