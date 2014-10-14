#pragma once


#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


template <typename PainterType>
class GLOPERATE_API PainterPlugin : public Plugin {


    public:
        PainterPlugin(const std::string & name, const std::string & description, const std::string & vendor, const std::string & version);
        virtual ~PainterPlugin();

        virtual Painter * createPainter(ResourceManager & resourceManager) const;


};


} // namespace gloperate


#include <gloperate/plugin/PainterPlugin.hpp>
