
#pragma once

#include <unordered_map>

#include <gloperate/gloperate_api.h>


namespace gloperate{

class InputEvent;
class AbstractMetaphor;

class GLOPERATE_API Mapping
{
public:
    void onEvent(unsigned int controlId, InputEvent * event);
    void addConnection(unsigned int controlId, AbstractMetaphor * metaphor);

protected:
    std::unordered_multimap<unsigned int, AbstractMetaphor *> m_mapping;

};

} // namespace gloperate
