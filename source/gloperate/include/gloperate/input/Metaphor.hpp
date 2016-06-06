
#include <gloperate/input/InputEvent.h>

namespace gloperate{

template <typename T>
Metaphor<T>::Metaphor(std::set<InputEvent::Type> types, handlerType handler)
: m_handler{handler}
, m_types{types}
{

}

template <typename T>
void Metaphor<T>::onEvent(InputEvent * event)
{
    if(m_types.count(event->type()))
        m_handler(event);
}

}
