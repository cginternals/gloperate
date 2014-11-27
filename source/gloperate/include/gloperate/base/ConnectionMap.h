#pragma once

#include <unordered_map>

#include <gloperate/gloperate_api.h>

#include <gloperate/base/Connection.h>


namespace gloperate
{

class ConnectionMap
{
public:
	ConnectionMap();
	~ConnectionMap();

	template <typename T>
	Connection & operator[](T * object);

protected:
    Connection & get(void * ptr);

protected:
	std::unordered_map<void *, Connection> m_connections;
};

} // namespace gloperate


#include <gloperate/base/ConnectionMap.hpp>
