#pragma once

#include <memory>

#include <gloperate/gloperate_api.h>


namespace gloperate
{

class AbstractSignal;

class GLOPERATE_API Connection
{
	friend class AbstractSignal;

public:
    typedef unsigned int Id;


protected:
    struct State
	{
		const AbstractSignal * signal;
		Id id;
	};

public:
	Connection();

	void disconnect();

	Id id() const;

protected:
    Connection(
        const AbstractSignal * signal
    ,   Id id);

	void detach();

protected:
	std::shared_ptr<State> m_state;
};

} // namespace gloperate
