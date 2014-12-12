#include <gloperate/base/Connection.h>

#include <gloperate/base/AbstractSignal.h>


namespace gloperate
{

Connection::Connection()
{
}

Connection::Connection(const AbstractSignal * signal, Id id)
:   m_state(new State { signal, id })
{
}

Connection::Id Connection::id() const
{
    return m_state ? m_state->id : 0;
}

void Connection::disconnect()
{
    if (m_state && m_state->signal)
    {
        m_state->signal->disconnect(*this);
        detach();
    }
}

void Connection::detach()
{
    if (m_state)
    {
        m_state->signal = nullptr;
    }
}

} // namespace gloperate
