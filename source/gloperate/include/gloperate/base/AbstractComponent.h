
#pragma once


#include <memory>

#include <cppexpose/plugin/AbstractComponent.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Environment;
class Stage;


/**
*  @brief
*    Component base class for gloperate classes
*/
template <class BaseType>
class AbstractComponent : public cppexpose::AbstractComponent
{
public:
    AbstractComponent();
    virtual ~AbstractComponent();

    virtual std::unique_ptr<BaseType> createInstance(gloperate::Environment * environment) = 0;
};


/**
*  @brief
*    Component base class for pipelines and stages
*/
template <>
class AbstractComponent<gloperate::Stage> : public cppexpose::AbstractComponent
{
public:
    AbstractComponent()
    {
    }

    virtual ~AbstractComponent()
    {
    }

    virtual gloperate::Stage * createInstance(gloperate::Environment * environment) = 0;
    virtual gloperate::Stage * createInstance(gloperate::Environment * environment, const std::string & name) = 0;
};


} // namespace gloperate


#include <gloperate/base/AbstractComponent.inl>
