
#pragma once


namespace gloperate
{


template <typename T>
Storer<T>::Storer(Environment * environment)
: AbstractStorer(environment)
{
}

template <typename T>
Storer<T>::~Storer()
{
}


} // namespace gloperate
