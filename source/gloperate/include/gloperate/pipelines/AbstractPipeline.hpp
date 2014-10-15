#pragma once

#include <gloperate/util/collection.hpp>

#include <gloperate/pipelines/AbstractPipeline.h>
#include <gloperate/pipelines/AbstractData.h>
#include <gloperate/pipelines/Data.h>

namespace gloperate
{

template<typename T, typename... Args>
void AbstractPipeline::addStages(T stage, Args... pipeline)
{
	addStage(stage);
	addStages(pipeline...);
}

template <typename T>
Data<T> * AbstractPipeline::createParameter(const std::string & name, const T & value)
{
    auto parameter = new Data<T>(value);
    addParameter(name, parameter);
    return parameter;
}

template <typename T>
Data<T> * AbstractPipeline::getParameter(const std::string & name) const
{
    return dynamic_cast<Data<T>*>(findParameter(name));
}

template <typename T>
Data<T> * AbstractPipeline::getParameter() const
{
    return dynamic_cast<Data<T>*>(collection::detect(m_parameters, [](AbstractData * parameter) { return dynamic_cast<Data<T>*>(parameter) != nullptr; }, nullptr));
}

template <typename T>
Data<T> * AbstractPipeline::getOutput(const std::string & name) const
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
Data<T> * AbstractPipeline::getOutput() const
{
    return dynamic_cast<Data<T>*>(collection::detect(allOutputs(), [](AbstractData * data) { return dynamic_cast<Data<T>*>(data) != nullptr; }, nullptr));
}

} // namespace gloperate
