
#pragma once


#include <cppexpose/plugin/Component.h>


namespace gloperate
{


class Environment;
class EnvironmentUser;


template <typename Type>
using IsGloperateComponent = typename std::enable_if<std::is_base_of<EnvironmentUser, Type>::value>::type;

}


namespace cppexpose
{


/**
*  @brief
*    Represents a component of type inherited from EnvironmentUser
*/
template <typename Type>
class TypedComponent<Type, gloperate::IsGloperateComponent<Type>> : public AbstractComponent
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

    virtual Type * createInstance(gloperate::Environment * environment) const = 0;
};


/**
*  @brief
*    Represents a concrete component of type inherited from EnvironmentUser
*/
template <typename Type, typename BaseType>
class Component<Type, BaseType, gloperate::IsGloperateComponent<BaseType>> : public TypedComponent<BaseType>
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

    virtual BaseType * createInstance(gloperate::Environment * environment) const override;
};


} // namespace cppexpose


#include <gloperate/base/Component.inl>
