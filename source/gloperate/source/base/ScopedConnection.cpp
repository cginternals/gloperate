#include <gloperate/base/ScopedConnection.h>


namespace gloperate
{

ScopedConnection::ScopedConnection()
{
}

ScopedConnection::ScopedConnection(const Connection & connection)
:   m_connection(connection)
{
}

ScopedConnection::~ScopedConnection()
{
    m_connection.disconnect();
}

void ScopedConnection::operator=(const Connection & connection)
{
    m_connection.disconnect();
    m_connection = connection;
}

} // namespace gloperate
