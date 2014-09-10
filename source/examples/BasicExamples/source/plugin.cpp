#include <vector>
#include <gloperate/plugin/plugin_api.h>
#include <gloperate/plugin/Plugin.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>
#include <basic-examples/CubeScape/CubeScape.h>


static std::vector<gloperate::Plugin *> g_plugins;


class Plugin_SimpleTexture : public gloperate::Plugin {
public:
    Plugin_SimpleTexture()
    : gloperate::Plugin(
        "Painter"
      , "SimpleTexture"
      , "Displays a simple generated texture on the screen"
      , "gloperate team"
      , "v1.0.0" )
    {
    }

    ~Plugin_SimpleTexture()
    {
    }

    virtual gloperate::Painter * createPainter() const
    {
        return new SimpleTexture();
    }
};

class Plugin_RotatingQuad : public gloperate::Plugin {
public:
    Plugin_RotatingQuad()
    : gloperate::Plugin(
        "Painter"
      , "RotatingQuad"
      , "Displays a rotating quad geometry, demonstrating animation and cameras"
      , "gloperate team"
      , "v1.0.0" )
    {
    }

    ~Plugin_RotatingQuad()
    {
    }

    virtual gloperate::Painter * createPainter() const
    {
        return new RotatingQuad();
    }
};

class Plugin_CubeScape : public gloperate::Plugin {
public:
    Plugin_CubeScape()
    : gloperate::Plugin(
        "Painter"
      , "CubeScape"
      , "Displays a CubeScape"
      , "gloperate team"
      , "v1.0.0" )
    {
    }

    ~Plugin_CubeScape()
    {
    }

    virtual gloperate::Painter * createPainter() const
    {
        return new CubeScape();
    }
};


extern "C" GLOPERATE_PLUGIN_API void initPlugin()
{
    g_plugins.push_back(new Plugin_SimpleTexture());
    g_plugins.push_back(new Plugin_RotatingQuad());
    g_plugins.push_back(new Plugin_CubeScape());
}

extern "C" GLOPERATE_PLUGIN_API int getNumOfPlugins()
{
    return (int)g_plugins.size();
}

extern "C" GLOPERATE_PLUGIN_API gloperate::Plugin * getPlugin(unsigned int index)
{
    if (index < (unsigned int)g_plugins.size()) {
        return g_plugins[index];
    } else {
        return nullptr;
    }
}

extern "C" GLOPERATE_PLUGIN_API void deinitPlugin()
{
    for (std::vector<gloperate::Plugin *>::iterator it = g_plugins.begin(); it != g_plugins.end(); ++it) {
        gloperate::Plugin * plugin = *it;
        delete plugin;
    }
    g_plugins.clear();
}
