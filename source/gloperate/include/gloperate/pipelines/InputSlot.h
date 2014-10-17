#pragma once

#include <gloperate/signals/ScopedConnection.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/AbstractInputSlot.h>
#include <gloperate/pipelines/Data.h>

namespace gloperate {

void GLOPERATE_API printIncompatibleMessage(const AbstractInputSlot* slot, const std::string& typeName,
                              const AbstractData & data);

template <typename T>
class InputSlot : public AbstractInputSlot
{
public:
	InputSlot();

    const T & data() const;
    const T & data(const T & defaultValue) const;
	const T & operator*() const;
	const T * operator->() const;

	virtual bool connectTo(const AbstractData & data) override;
    virtual bool matchType(const AbstractData & data) override;

	const Data<T> & operator=(const Data<T> & data);
    InputSlot & operator=(InputSlot & slot);

    virtual const AbstractData * connectedData() const override;

protected:
	const Data<T>* m_data;
	gloperate::ScopedConnection m_connection;

    static const T s_defaultValue;

	void connect(const Data<T> & data);

private:
};

} // namespace gloperate

#include <gloperate/pipelines/InputSlot.hpp>
