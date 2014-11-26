#pragma once

#include <gloperate/gloperate_api.h>


namespace gloperate
{

enum class SourceType : char
{
    Keyboard
,   Mouse
,   Timer
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
