
#include <gloperate/input/Mapping.h>
#include <gloperate/input/Metaphor.h>

namespace gloperate{

void Mapping::onEvent(unsigned int controlId, InputEvent * event)
{
    auto range = m_mapping.equal_range(controlId);

    for(auto it = range.first; it != range.second; ++it)
    {
        it->second->onEvent(event);
    }
}

} // namespace gloperate
