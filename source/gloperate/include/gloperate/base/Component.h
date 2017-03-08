
#pragma once


#include <gloperate/gloperate_api.h>

#include <gloperate/base/AbstractComponent.h>


namespace gloperate
{


class Environment;
class Stage;


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

    virtual std::unique_ptr<BaseType> createInstance(gloperate::Environment * environment) override;
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

    virtual std::unique_ptr<gloperate::Stage> createInstance(gloperate::Environment * environment) override;
    virtual std::unique_ptr<gloperate::Stage> createInstance(gloperate::Environment * environment, const std::string & name) override;
};


} // namespace gloperate


#include <gloperate/base/Component.inl>
