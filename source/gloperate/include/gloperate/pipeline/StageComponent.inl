
#pragma once


namespace cppexpose
{


template <typename Type>
Component<Type, gloperate::Stage>::Component(
  const std::string & name
, const std::string & description
, const std::string & type
, const std::string & tags
, const std::string & icon
, const std::string & annotations
, const std::string & vendor
, const std::string & version)
: TypedComponent<gloperate::Stage>(name, description, type, tags, icon, annotations, vendor, version)
{
}

template <typename Type>
Component<Type, gloperate::Stage>::~Component()
{
}

template <typename Type>
gloperate::Stage * Component<Type, gloperate::Stage>::createInstance(gloperate::Environment * environment) const
{
    return new Type(environment);
}

template <typename Type>
gloperate::Stage * Component<Type, gloperate::Stage>::createInstance(gloperate::Environment * environment, const std::string & name) const
{
    return new Type(environment, name);
}


} // namespace cppexpose
