
#pragma once


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

    virtual BaseType * createInstance(gloperate::Environment * environment) = 0;
};


/**
*  @brief
*    Component class for gloperate classes
*/
template <class BaseType, class Type>
class Component : public AbstractComponent<BaseType>
{
public:
    Component();
    virtual ~Component();

    virtual BaseType * createInstance(gloperate::Environment * environment) override;
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


/**
*  @brief
*    Component class for pipelines and stages
*/
template <class Type>
class Component<gloperate::Stage, Type> : public AbstractComponent<gloperate::Stage>
{
public:
    Component();
    virtual ~Component();

    virtual gloperate::Stage * createInstance(gloperate::Environment * environment) override;
    virtual gloperate::Stage * createInstance(gloperate::Environment * environment, const std::string & name) override;
};


} // namespace gloperate


#include <gloperate/base/Component.inl>
