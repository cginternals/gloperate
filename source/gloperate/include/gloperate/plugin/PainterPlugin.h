#pragma once

#include <gloperate/plugin/Plugin.h>


namespace gloperate
{

/** /brief Convenience class to describe a plugin for a painter
*/
template <typename PainterType>
class PainterPlugin : public Plugin 
{
public:
    PainterPlugin(
        const std::string & name
    ,   const std::string & description
    ,   const std::string & vendor
    ,   const std::string & version);
    virtual ~PainterPlugin();

	virtual Painter * createPainter(ResourceManager & resourceManager, std::unique_ptr<gloperate_qt::QtOpenGLWindow>& canvas) const;
};

} // namespace gloperate


#include <gloperate/plugin/PainterPlugin.hpp>
