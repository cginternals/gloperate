
#pragma once


#include <gloperate/pipeline/Parameter.h>


namespace gloperate
{


template <typename T>
Parameter<T>::Parameter(const std::string & name, Stage * parent, const T & value)
: DataSlot<T>(value)
{
    // Do not add property to object, yet. Just initialize the property itself
    this->initProperty(name, nullptr, cppexpose::PropertyOwnership::None);

    // Register parameter, will also add parameter as a property
    this->initDataSlot(SlotType::Parameter, parent, cppexpose::PropertyOwnership::None);
}

template <typename T>
Parameter<T>::~Parameter()
{
}

template <typename T>
bool Parameter<T>::isValid() const
{
    // Parameters are always valid
    return true;
}


} // namespace gloperate
