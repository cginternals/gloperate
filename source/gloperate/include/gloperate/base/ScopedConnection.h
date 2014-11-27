#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/base/Connection.h>


namespace gloperate 
{

class GLOPERATE_API ScopedConnection
{
public:
	ScopedConnection();
	ScopedConnection(const Connection & connection);
	~ScopedConnection();

	void operator=(const Connection & connection);

protected:
	Connection m_connection;
};

} // namespace gloperate
