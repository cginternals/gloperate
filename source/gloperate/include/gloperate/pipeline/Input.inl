
#pragma once


namespace gloperate
{


template <typename T>
Input<T>::Input(const std::string & name, Stage * parent, const T & value)
: Slot<T>(SlotType::Input, name, parent, value)
{
}

template <typename T>
Input<T>::Input(const std::string & name, const T & value)
: Slot<T>(SlotType::Input, name, value)
{
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
