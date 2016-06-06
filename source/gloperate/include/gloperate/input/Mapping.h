
#pragma once

#include <unordered_map>

#include <gloperate/gloperate_api.h>


namespace gloperate{

class InputEvent;
class AbstractMetaphor;

class GLOPERATE_API Mapping
{
    void onEvent(unsigned int controlId, InputEvent * event);

protected:
    std::unordered_map<unsigned int, AbstractMetaphor *> m_mapping;

};

} // namespace gloperate
