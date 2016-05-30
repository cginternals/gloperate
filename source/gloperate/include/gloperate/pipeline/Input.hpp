
#pragma once


#include <gloperate/pipeline/Input.h>


namespace gloperate
{


template <typename T>
Input<T>::Input(const std::string & name, Stage * parent, const T & value)
: InputSlot<T, AbstractInput>(value)
{
    // Do not add property to group yet, only initialize the property itself
    this->initProperty(name, nullptr, cppexpose::PropertyOwnership::None);

    // Register input, will also add input as a property
    this->initInput(parent, cppexpose::PropertyOwnership::None);
}

template <typename T>
Input<T>::~Input()
{
}


} // namespace gloperate
