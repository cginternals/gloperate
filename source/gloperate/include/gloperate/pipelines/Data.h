#pragma once

#include <string>

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/AbstractData.h>

namespace gloperate {

template <typename T>
class Data : public virtual AbstractData
{
public:
	Data();
	explicit Data(const T & data);

	T & data();
	const T & data() const;

	T & operator*();
	const T & operator*() const;
	T * operator->();
	const T * operator->() const;

	operator const T &() const;

	const T & operator=(const T & value);

    void setData(const T & value);

    virtual std::string type() const override { return typeid(T).name(); }
protected:
	T m_data;
};

} // namespace gloperate

#include <gloperate/pipelines/Data.hpp>
