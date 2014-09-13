#include <vector>
#include <gloperate/plugin/plugin_api.h>
#include <gloperate/plugin/PainterPlugin.h>
#include <basic-examples/SimpleTexture/SimpleTexture.h>
#include <basic-examples/RotatingQuad/RotatingQuad.h>
#include <basic-examples/CubeScape/CubeScape.h>


static std::vector<gloperate::Plugin *> g_plugins;


extern "C" GLOPERATE_PLUGIN_API void initPluginLibrary()
{
    g_plugins.push_back(new gloperate::PainterPlugin<SimpleTexture>(
                            "SimpleTexture"
                          , "Displays a simple generated texture on the screen"
                          , "gloperate team"
                          , "v1.0.0" ) );

    g_plugins.push_back(new gloperate::PainterPlugin<RotatingQuad>(
                            "RotatingQuad"
                          , "Displays a rotating quad geometry, demonstrating animation and cameras"
                          , "gloperate team"
                          , "v1.0.0" ) );

    g_plugins.push_back(new gloperate::PainterPlugin<CubeScape>(
                            "CubeScape"
                          , "Displays a CubeScape"
                          , "gloperate team"
                          , "v1.0.0" ) );
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

extern "C" GLOPERATE_PLUGIN_API void deinitPluginLibrary()
{
    for (std::vector<gloperate::Plugin *>::iterator it = g_plugins.begin(); it != g_plugins.end(); ++it) {
        gloperate::Plugin * plugin = *it;
        delete plugin;
    }
    g_plugins.clear();
}
