
#pragma once


#include <gloperate/pipeline/ProxyOutput.h>


namespace gloperate
{


template <typename T>
ProxyOutput<T>::ProxyOutput(Stage * parent, const std::string & name, const T & value)
: InputSlot<T, AbstractProxyOutput>(parent, name, value)
{
}

template <typename T>
ProxyOutput<T>::~ProxyOutput()
{
}


} // namespace gloperate
