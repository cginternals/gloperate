
#include <gloperate/plugin/PainterPlugin.h>


namespace gloperate
{


AbstractPainterPlugin::AbstractPainterPlugin(
    const std::string & name
  , const std::string & description
  , const std::string & vendor
  , const std::string & version)
: Plugin("Painter", name, description, vendor, version)
{
}

AbstractPainterPlugin::~AbstractPainterPlugin()
{
}


} // namespace gloperate
