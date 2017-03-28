
#pragma once


#include <typeinfo>

#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


template <typename T>
Input<T> * Stage::CreateConnectedInputProxy::operator<<(Slot<T> & source)
{
    ++m_createdCount;
    return m_stage->createConnectedInput(m_name, source);
}

template <typename T>
std::vector<Input<T> *> Stage::inputs() const
{
    auto result = std::vector<Input<T> *>{};
    
    // We do not reserve a heuristically derived number of elements as we assume
    // the vector growing strategy would handle most cases efficiently
    // result.reserve(inputs().size() / 2);
    
    for (auto input : inputs())
    {
        if (input->type() == typeid(T))
        {
            result.push_back(static_cast<Input<T> *>(input));
        }
    }

    return result;
}

template <typename T>
Input<T> * Stage::createInput(const std::string & name, const T & defaultValue)
{
    auto input = cppassist::make_unique<Input<T>>(name, defaultValue);
    auto inputPtr = input.get();

    this->addInput(std::move(input));

    return inputPtr;
}

template <typename T>
Input<T> * Stage::createConnectedInput(const std::string & name, Slot<T> & source)
{
    auto input = createInput<T>(name);
    input->connect(&source);

    return input;
}

template <typename T>
std::vector<Output<T> *> Stage::outputs() const
{
    auto result = std::vector<Output<T> *>{};
    
    // We do not reserve a heuristically derived number of elements as we assume
    // the vector growing strategy would handle most cases efficiently
    // result.reserve(outputs().size() / 2);
    
    for (auto output : outputs())
    {
        if (output->type() == typeid(T))
        {
            result.push_back(static_cast<Output<T> *>(output));
        }
    }

    return result;
}

template <typename T>
Output<T> * Stage::createOutput(const std::string & name, const T & defaultValue)
{
    auto output = cppassist::make_unique<Output<T>>(name, defaultValue);
    auto outputPtr = output.get();

    this->addOutput(std::move(output));

    return outputPtr;
}

template <typename T>
Slot<T> * Stage::createSlot(const std::string & slotType, const std::string & name, const T & defaultValue)
{
         if (slotType == "Input")  return createInput <T>(name, defaultValue);
    else if (slotType == "Output") return createOutput<T>(name, defaultValue);

    return nullptr;
}


} // namespace gloperate
