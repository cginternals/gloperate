
#pragma once


#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


template <typename T>
Input<T> * Stage::createInput(const std::string & name, const T & defaultValue)
{
    auto input = new Input<T>(name, defaultValue);
    this->addInput(input, cppexpose::PropertyOwnership::Parent);

    return input;
}

template <typename T>
Output<T> * Stage::createOutput(const std::string & name, const T & defaultValue)
{
    auto output = new Output<T>(name, defaultValue);
    this->addOutput(output, cppexpose::PropertyOwnership::Parent);

    return output;
}

template <typename T>
Slot<T> * Stage::createSlot(const std::string & slotType, const std::string & name, const T & defaultValue)
{
         if (slotType == "Input")  return createInput <T>(name, defaultValue);
    else if (slotType == "Output") return createOutput<T>(name, defaultValue);

    return nullptr;
}


} // namespace gloperate
