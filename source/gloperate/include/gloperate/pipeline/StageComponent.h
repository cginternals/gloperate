
#pragma once


#include <cppexpose/plugin/Component.h>

#include <gloperate/gloperate_api.h>


namespace gloperate {
    class Environment;
    class Stage;
}


namespace cppexpose
{


/**
*  @brief
*    Represents a component of type gloperate::Stage
*/
template <>
class GLOPERATE_API TypedComponent<gloperate::Stage> : public AbstractComponent
{
public:
    TypedComponent(
      const std::string & name
    , const std::string & description
    , const std::string & type
    , const std::string & tags
    , const std::string & icon
    , const std::string & annotations
    , const std::string & vendor
    , const std::string & version);

    virtual ~TypedComponent();

    virtual gloperate::Stage * createInstance(gloperate::Environment * environment) const = 0;
    virtual gloperate::Stage * createInstance(gloperate::Environment * environment, const std::string & name) const = 0;
};


/**
*  @brief
*    Represents a concrete component of type gloperate::Stage
*/
template <typename Type>
class Component<Type, gloperate::Stage> : public TypedComponent<gloperate::Stage>
{
public:
    Component(
      const std::string & name
    , const std::string & description
    , const std::string & type
    , const std::string & tags
    , const std::string & icon
    , const std::string & annotations
    , const std::string & vendor
    , const std::string & version);

    virtual ~Component();

    virtual gloperate::Stage * createInstance(gloperate::Environment * environment) const override;
    virtual gloperate::Stage * createInstance(gloperate::Environment * environment, const std::string & name) const override;
};


} // namespace cppexpose


#include <gloperate/pipeline/StageComponent.inl>
