
#pragma once


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
void Parameter<T>::invalidate()
{
    // Parameters are always valid
}

template <typename T>
bool Parameter<T>::isValid() const
{
    // Parameters are always valid
    return true;
}

template <typename T>
void Parameter<T>::onRequiredChanged()
{
    // Doesn't need to propagate change
}

template <typename T>
void Parameter<T>::onValueChanged(const T & value)
{
    // Emit signal
    this->valueChanged(value);

    // Inform parent stage
    if (Stage * stage = this->parentStage())
    {
        stage->inputValueChanged(this);
    }
}


} // namespace gloperate
