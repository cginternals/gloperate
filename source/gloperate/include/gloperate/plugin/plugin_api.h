
#pragma once


#include <vector>

#include <gloperate/plugin/PainterPlugin.h>


#ifdef _MSC_VER
#   define GLOPERATE_PLUGIN_API __declspec(dllexport)
#elif __GNUC__
#   define GLOPERATE_PLUGIN_API __attribute__ ((visibility ("default")))
#else
#   define GLOPERATE_PLUGIN_API
#endif


#define GLOPERATE_PLUGIN_LIBRARY \
    static std::vector<gloperate::Plugin *> g_plugins; \
    \
    extern "C" GLOPERATE_PLUGIN_API void initialize() \
    {

#define GLOPERATE_PAINTER_PLUGIN(CLASS, NAME, DESCRIPTION, VENDOR, VERSION) \
        g_plugins.push_back(new gloperate::PainterPlugin< CLASS >(NAME, DESCRIPTION, VENDOR, VERSION));

#define GLOPERATE_PLUGIN_LIBRARY_END \
    } \
    \
    extern "C" GLOPERATE_PLUGIN_API int numPlugins() \
    { \
        return (int)g_plugins.size(); \
    } \
    \
    extern "C" GLOPERATE_PLUGIN_API gloperate::Plugin * plugin(unsigned int index) \
    { \
        if (index < (unsigned int)g_plugins.size()) \
            return g_plugins[index]; \
        \
        return nullptr; \
    } \
    \
    extern "C" GLOPERATE_PLUGIN_API void deinitialize() \
    { \
        std::vector<gloperate::Plugin *>::iterator i = g_plugins.begin(); \
        const std::vector<gloperate::Plugin *>::const_iterator iEnd = g_plugins.cend(); \
        \
        for (; i != iEnd; ++i) \
        { \
            gloperate::Plugin * plugin = *i; \
            delete plugin; \
        } \
        g_plugins.clear(); \
    }
