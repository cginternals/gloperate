#pragma once

namespace gloperate
{

template <typename T>
gloperate::Data<T> * AbstractOutputCapability::getOutput(const std::string & name) const
{
    for (AbstractData* output : findOutputs(name))
    {
        Data<T>* data = dynamic_cast<Data<T>*>(output);
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