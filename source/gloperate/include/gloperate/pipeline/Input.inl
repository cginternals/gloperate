
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
void Input<T>::onValueInvalidated()
{
    cppassist::debug(3, "gloperate") << this->qualifiedName() << ": input invalidated";

    std::lock_guard<std::recursive_mutex> lock(this->m_cycleMutex);

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
        cppassist::debug(4, "gloperate") << this->qualifiedName() << ": detected cyclic dependency";
        return;
    }

    // Raise guard
    this->m_cycleGuard[this_id] = true;

    // Emit signal
    this->valueInvalidated();

    this->setChanged(true);

    // Inform parent stage
    if (Stage * stage = this->parentStage())
    {
        stage->inputValueInvalidated(this);
    }

    // Reset guard
    this->m_cycleGuard[this_id] = false;
}

template <typename T>
void Input<T>::onValueChanged(const T & value)
{
    cppassist::debug(3, "gloperate") << this->qualifiedName() << ": input changed value";

    this->setChanged(true);

    // Emit signal
    this->valueChanged(value);

    // Inform parent stage
    if (Stage * stage = this->parentStage())
    {
        stage->inputValueChanged(this);
    }
}

template <typename T>
void Input<T>::onOptionChanged(const std::string &)
{
    // Inform parent stage
    if (Stage * stage = this->parentStage())
    {
        stage->inputOptionsChanged(this);
    }
}

template <typename T>
template <typename U, typename Enable>
Input<T> & Input<T>::operator=(const U & value)
{
    this->setValue(value);

    return *this;
}


} // namespace gloperate
