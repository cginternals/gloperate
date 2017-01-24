
#pragma once


namespace cppexpose
{


template <typename Type>
TypedComponent<Type, gloperate::IsGloperateComponent<Type>>::TypedComponent(
  const std::string & name
, const std::string & description
, const std::string & type
, const std::string & tags
, const std::string & icon
, const std::string & annotations
, const std::string & vendor
, const std::string & version)
: AbstractComponent(name, description, type, tags, icon, annotations, vendor, version)
{
}


template <typename Type, typename BaseType>
Component<Type, BaseType, gloperate::IsGloperateComponent<BaseType>>::Component(
  const std::string & name
, const std::string & description
, const std::string & type
, const std::string & tags
, const std::string & icon
, const std::string & annotations
, const std::string & vendor
, const std::string & version)
: TypedComponent<BaseType>(name, description, type, tags, icon, annotations, vendor, version)
{
}

template <typename Type, typename BaseType>
BaseType * Component<Type, BaseType, gloperate::IsGloperateComponent<BaseType>>::createInstance(gloperate::Environment * environment) const
{
    return new Type(environment);
}


} // namespace cppexpose
