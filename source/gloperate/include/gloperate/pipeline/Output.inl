
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
    // Get current thread ID
    std::thread::id this_id = std::this_thread::get_id();

    // Initialize guard for the current thread
    if (this->m_cycleGuard.find(this_id) == this->m_cycleGuard.end())
    {
        this->m_cycleGuard[this_id] = false;
    }

    // Check if this slot has already been invoked in the current recursion
    if (this->m_cycleGuard[this_id])
    {
        // Reset guard
        this->m_cycleGuard[this_id] = false;

        // Stop recursion here to avoid endless recursion
        return;
    }

    // Raise guard
    this->m_cycleGuard[this_id] = true;

    // Emit signal
    this->valueChanged(value);

    // Reset guard
    this->m_cycleGuard[this_id] = false;
}


} // namespace gloperate
