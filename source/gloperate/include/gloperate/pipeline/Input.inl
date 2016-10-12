
#pragma once


#include <cppassist/logging/logging.h>


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
        cppassist::warning() << "detected cyclic dependency for " << this->qualifiedName();
        return;
    }

    // Raise guard
    this->m_cycleGuard[this_id] = true;

    // Emit signal
    this->valueChanged(value);

    // Inform parent stage
    if (Stage * stage = this->parentStage())
    {
        stage->inputValueChanged(this);
    }

    // Reset guard
    this->m_cycleGuard[this_id] = false;
}


} // namespace gloperate
