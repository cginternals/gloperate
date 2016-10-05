
#pragma once


namespace gloperate
{


template <typename T>
Output<T>::Output(const std::string & name, Stage * parent, const T & value)
: Slot<T>(SlotType::Output, name, parent, value)
{
    // Outputs are invalid until their value is set once
    this->m_valid = false;
}

template <typename T>
Output<T>::Output(const std::string & name, const T & value)
: Slot<T>(SlotType::Output, name, value)
{
    // Outputs are invalid until their value is set once
    this->m_valid = false;
}

template <typename T>
Output<T>::~Output()
{
}

template <typename T>
void Output<T>::onRequiredChanged()
{
    if (this->isConnected())
    {
        // Promote required-flag to source slot
        this->promoteRequired();
    }
    else
    {
        // Inform parent stage
        if (Stage * stage = this->parentStage())
        {
            stage->outputRequiredChanged(this);
        }
    }
}

template <typename T>
void Output<T>::onValueChanged(const T & value)
{
    // Emit signal
    this->valueChanged(value);
}


} // namespace gloperate
