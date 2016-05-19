
#pragma once


#include <gloperate/pipeline/Data.h>


namespace gloperate
{


template <typename T, typename BASE>
Data<T, BASE>::Data(Stage * parent, const std::string & name, const T & value)
: cppexpose::DynamicProperty<T, BASE>(parent, name, value)
{
}

template <typename T, typename BASE>
Data<T, BASE>::~Data()
{
}


} // namespace gloperate
