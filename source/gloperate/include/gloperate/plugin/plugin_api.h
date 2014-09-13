/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
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
	extern "C" GLOPERATE_PLUGIN_API void initPluginLibrary() \
	{ \

#define GLOPERATE_PLUGIN(CLASS, NAME, DESCRIPTION, VENDOR, VERSION) \
    	g_plugins.push_back(new gloperate::PainterPlugin< CLASS >(NAME, DESCRIPTION, VENDOR, VERSION));

#define GLOPERATE_PLUGIN_LIBRARY_END \
	} \
	\
	extern "C" GLOPERATE_PLUGIN_API int getNumOfPlugins() \
	{ \
	    return (int)g_plugins.size(); \
	} \
	\
	extern "C" GLOPERATE_PLUGIN_API gloperate::Plugin * getPlugin(unsigned int index) \
	{ \
	    if (index < (unsigned int)g_plugins.size()) { \
	        return g_plugins[index]; \
	    } else { \
	        return nullptr; \
	    } \
	} \
	\
	extern "C" GLOPERATE_PLUGIN_API void deinitPluginLibrary() \
	{ \
	    for (std::vector<gloperate::Plugin *>::iterator it = g_plugins.begin(); it != g_plugins.end(); ++it) { \
	        gloperate::Plugin * plugin = *it; \
	        delete plugin; \
	    } \
	    g_plugins.clear(); \
	}
