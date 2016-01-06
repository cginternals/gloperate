
#pragma once


#include <gloperate/base/collection.hpp>
#include <gloperate/pipeline/Data.h>


namespace gloperate
{


template <typename T>
gloperate::Data<T> * AbstractOutputCapability::getOutput(const std::string & name) const
{
    for (AbstractData* output : findOutputs(name))
    {
        auto data = dynamic_cast<Data<T>*>(output);
        if (data)
        {
            return data;
        }
    }

    return nullptr;
}

template <typename T>
gloperate::Data<T> * AbstractOutputCapability::getOutput() const
{
    return dynamic_cast<Data<T>*>(collection::detect(allOutputs(), [](AbstractData * data) { return dynamic_cast<Data<T>*>(data) != nullptr; }, nullptr));
}


} // namespace gloperate
