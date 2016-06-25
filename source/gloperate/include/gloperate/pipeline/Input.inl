
#pragma once


namespace gloperate
{


template <typename T>
Input<T>::Input(const std::string & name, Stage * parent, const T & value)
: InputSlot<T>(value)
{
    // Do not add property to object, yet. Just initialize the property itself
    this->initProperty(name, nullptr, cppexpose::PropertyOwnership::None);

    // Register input, will also add input as a property
    this->initInputSlot(SlotType::Input, parent, cppexpose::PropertyOwnership::None);
}

template <typename T>
Input<T>::~Input()
{
}

template <typename T>
void Input<T>::onValueChanged(const T & value)
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
