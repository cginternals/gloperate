#pragma once

#include <gloperate/plugin/PainterPlugin.h>


namespace gloperate
{

template <typename PainterType>
PainterPlugin<PainterType>::PainterPlugin(
    const std::string & name
,   const std::string & description
,   const std::string & vendor
,   const std::string & version)
: Plugin("Painter", name, description, vendor, version)
{
}

template <typename PainterType>
PainterPlugin<PainterType>::~PainterPlugin()
{
}

template <typename PainterType>
Painter * PainterPlugin<PainterType>::createPainter(ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas) const
{
    return new PainterType(resourceManager);
}

} // namespace gloperate
