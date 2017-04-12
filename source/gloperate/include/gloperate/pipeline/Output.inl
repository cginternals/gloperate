
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
void Output<T>::onValueInvalidated()
{
    cppassist::debug(3, "gloperate") << this->qualifiedName() << ": output invalidated";

    // Emit signal
    this->valueInvalidated();
}


template <typename T>
void Output<T>::onValueChanged(const T & value)
{
    cppassist::debug(3, "gloperate") << this->qualifiedName() << ": output changed value";
    
    // Emit signal
    this->valueChanged(value);
}

template <typename T>
template <typename U, typename Enable>
Output<T> & Output<T>::operator=(const U & value)
{
    this->setValue(value);

    return *this;
}


} // namespace gloperate
