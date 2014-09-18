#pragma once

#include <gloperate/gloperate_api.h>

#include <gloperate/pipelines/AbstractParameter.h>
#include <gloperate/pipelines/Data.h>

namespace gloperate {

template <typename T>
class Parameter : public virtual AbstractParameter, public Data<T>
{
public:
	Parameter();
	explicit Parameter(const T & data);

	const T & operator=(const T & value);
};

} // namespace gloperate

#include <gloperate/pipelines/Parameter.hpp>
