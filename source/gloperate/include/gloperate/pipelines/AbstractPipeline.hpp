#pragma once

#include <gloperate/util/collection.hpp>

#include <gloperate/pipelines/AbstractPipeline.h>
#include <gloperate/pipelines/AbstractParameter.h>
#include <gloperate/pipelines/Parameter.h>

namespace gloperate
{

template<typename T, typename... Args>
void AbstractPipeline::addStages(T stage, Args... pipeline)
{
	addStage(stage);
	addStages(pipeline...);
}

template <typename T>
Parameter<T> * AbstractPipeline::createParameter(const std::string & name, const T & value)
{
    auto parameter = new Parameter<T>(value);
    addParameter(name, parameter);
    return parameter;
}

template <typename T>
Parameter<T> * AbstractPipeline::getParameter(const std::string & name)
{
    return dynamic_cast<Parameter<T>*>(findParameter(name));
}

template <typename T>
Parameter<T> * AbstractPipeline::getParameter()
{
    return dynamic_cast<Parameter<T>*>(collection::detect(m_parameters, [](AbstractParameter * parameter) { return dynamic_cast<Parameter<T>*>(parameter) != nullptr; }, nullptr));
}

template <typename T>
Data<T> * AbstractPipeline::getOutput(const std::string & name)
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
Data<T> * AbstractPipeline::getOutput()
{
    return dynamic_cast<Data<T>*>(collection::detect(allOutputs(), [](AbstractData * data) { return dynamic_cast<Data<T>*>(data) != nullptr; }, nullptr));
}

} // namespace gloperate
