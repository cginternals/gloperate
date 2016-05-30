
#include <gloperate/pipeline/StageComponent.h>


namespace cppexpose
{


TypedComponent<gloperate::Stage>::TypedComponent(
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

TypedComponent<gloperate::Stage>::~TypedComponent()
{
}


} // namespace cppexpose
