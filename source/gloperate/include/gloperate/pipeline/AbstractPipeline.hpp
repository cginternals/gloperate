
#pragma once


#include <gloperate/base/collection.hpp>

#include <gloperate/pipeline/AbstractPipeline.h>
#include <gloperate/pipeline/AbstractData.h>
#include <gloperate/pipeline/Data.h>


namespace gloperate
{


template<typename... Args>
void AbstractPipeline::addStages(AbstractStage * stage, Args... pipeline)
{
    addStage(stage);
    addStages(std::forward<Args>(pipeline)...);
}

template <typename T>
Data<T> * AbstractPipeline::addConstantParameter(const T & value)
{
    auto constant = new Data<T>(value);

    m_constantParameters.push_back(constant);

    return constant;
}

template <typename T>
Data<T> * AbstractPipeline::getParameter(const std::string & name) const
{
    return dynamic_cast<Data<T> *>(findParameter(name));
}

template <typename T>
Data<T> * AbstractPipeline::getParameter() const
{
    return dynamic_cast<Data<T> *>(collection::detect(m_parameters, [](AbstractData * parameter) { return dynamic_cast<Data<T> *>(parameter) != nullptr; }, nullptr));
}

template <typename T>
Data<T> * AbstractPipeline::getOutput(const std::string & name) const
{
    for (AbstractData * output : findOutputs(name))
    {
        Data<T>* data = dynamic_cast<Data<T> *>(output);
        if (data)
        {
            return data;
        }
    }

    return nullptr;
}

template <typename T>
Data<T> * AbstractPipeline::getOutput() const
{
    return dynamic_cast<Data<T> *>(collection::detect(allOutputs(), [](AbstractData * data) { return dynamic_cast<Data<T> *>(data) != nullptr; }, nullptr));
}


} // namespace gloperate
