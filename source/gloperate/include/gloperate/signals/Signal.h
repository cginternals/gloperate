#pragma once

#include <functional>
#include <unordered_map>

#include <gloperate/gloperate_api.h>
#include <gloperate/signals/AbstractSignal.h>


namespace gloperate
{

template <typename... Arguments>
class Signal : public AbstractSignal
{
public:
	typedef std::function<void(Arguments...)> Callback;

	Signal();

	void fire(Arguments... arguments);
	void operator()(Arguments... arguments);

	Connection connect(Callback callback) const;
	Connection connect(Signal & signal) const;

    template <class T>
	Connection connect(T * object, void (T::*method)(Arguments...)) const;

	void block();
	void unblock();

	Connection onFire(std::function<void()> callback) const;

protected:
	virtual void disconnectId(Connection::Id id) const override;

protected:
	mutable std::unordered_map<Connection::Id, Callback> m_callbacks;
	bool m_blocked;
};

} // namespace gloperate

#include <gloperate/signals/Signal.hpp>
