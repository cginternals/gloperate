#include <gloperate/base/ConnectionMap.h>


namespace gloperate
{

ConnectionMap::ConnectionMap()
{
}

ConnectionMap::~ConnectionMap()
{
    for (std::pair<void *, Connection> pair: m_connections)
    {
        pair.second.disconnect();
    }
}

Connection & ConnectionMap::get(void * ptr)
{
    return m_connections[ptr];
}

} // namespace gloperate
